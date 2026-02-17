#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <utility>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cctype>
#include <fstream>
#include <sstream>
#include <queue>
#include "md5.h"

using namespace std;


enum class passwordError
{
    Upper_Case_Missing,
    Too_Few_Characters,
    Lower_Case_Missing,
    Number_Missing,
    Special_Sign_Missing,
    Invalid_Username,
    Empty_User,
    Username_Taken,
    Username_Approved,
    Password_Approved

};

struct UserData
{
    string salt;
    string hash;
};


int intControl()
{
    
    int x;
    while(1)
    {
        std::cout<<"Ange talet: "<<std::endl;
        std::cin>>x;
        if(std::cin.fail())
        {
            std::cout<<"Du har angivit fel inmatning! \n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        else
        {
            std::cout<<"Du har angivit: "<<x<<std::endl;
            break;
        }
    }
    return x;
}

void goNext()
{
    std::cout<<" \nTryck ENTER för att gå vidare"<<std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}


passwordError userControl (map<string, UserData>& key, std::string name)
{
    if(name.empty())
    {
        return passwordError::Empty_User;
    }

    size_t pos = name.find('@');
    if(pos == string::npos)
    {
        return passwordError::Invalid_Username;
    }
    size_t pos1 = name.find('.');
    if(pos1 == string::npos)
    {
        return passwordError::Invalid_Username;
    }
    else if((pos1 +1) == string::npos)
    {
        return passwordError::Invalid_Username;
    }
    else if((pos1 -1) == string::npos)
    /* här kan faktistk det finnas en isiffra innan, michael1321@
    */

    {
        return passwordError::Invalid_Username;
    }
    auto it = key.find(name);
    if(it != key.end())
    {
        return passwordError::Username_Taken;
    }
    return passwordError::Username_Approved;
}

passwordError userMenu (map<string, UserData>& map, string& name)
{
    
    bool flag = true;
    while(flag)
    {
        cout<<"Ange din username: \n";
        cin>>name;
        passwordError resultat = userControl(map, name);
        switch(resultat)
        {
            case passwordError::Empty_User:
            cout<<"Tom input, försök igen! \n";
            continue;

            case passwordError::Invalid_Username:
            cout<<"Ditt användarnamn kan sakna: @, . eller bokstäver innan eller efter . \n";
            continue;

            case passwordError::Username_Taken:
            cout<<"Användarnamnet är redan upptaget! \n";
            continue;

            case passwordError::Username_Approved:
            cout<<"Tack, vi har nu lagt till ditt användarnamn \n";
            break;
        }
        flag = false;
    }
    return passwordError::Username_Approved;
}

vector<passwordError> pwControl(string pw)
{
    bool hasUpperCase = false;
    bool hasNumbr = false;
    bool hasSpecial = false;
    bool hasLowerCase = false;
    bool TooFewChar = false;
    vector<passwordError> lista;

    for(auto x: pw)
    {
        if(isupper(x))
        {
            hasUpperCase = true;
            
        }
        if(isdigit(x))
        {
            hasNumbr = true;
            
        }
        if(ispunct(x))
        {
            hasSpecial = true;
            
        }
        if(islower(x))
        {
            hasLowerCase = true;
            
        }
        if(pw.size() < 8)
        {
            TooFewChar = true;
        }
    }
    if(!hasLowerCase)
    {
        lista.emplace_back(passwordError::Lower_Case_Missing);
    }
    if(!hasUpperCase)
    {
        lista.emplace_back(passwordError::Upper_Case_Missing);
    }
    if(!hasNumbr)
    {
        lista.emplace_back(passwordError::Number_Missing);
    }
    if(!hasSpecial)
    {
        lista.emplace_back(passwordError::Special_Sign_Missing);
    }
    if(TooFewChar)
    {
        lista.emplace_back(passwordError::Too_Few_Characters);
    }

    return lista;

}

string createSalt()
{
    string text = "OgaBogaStefan_Holmberg1337_HellO0W0rLd!Omg-i-N33D-He3lLp";
    string salt = "";
    int length = 6;
    for(int i = 0; i < length;i++)
    {
        int index = rand() % text.size();
        salt += text[index];
    }
    return salt;


}

string enumConverter(passwordError pw)
{
    switch(pw)
    {
       case passwordError::Lower_Case_Missing:
       return "Saknar liten bokstav";
       break;

       case passwordError::Number_Missing:
       return "Saknar en siffra";
       break;

       case passwordError::Too_Few_Characters:
       return "För få bokstäver";
       break;

       case passwordError::Upper_Case_Missing:
       return "Saknar stor bokstav";
       break;

       case passwordError::Special_Sign_Missing:
       return "Saknar specialtecken";
       break;

    }
    return "";
}

string pwMenu(string& password)
{
    bool flag = true;
    while(flag)
    {
    string hash;
    vector<passwordError> lista;
    cout<<"Ange ditt lösenord: \n";
    cout<<"Lösenordet ska inkludera: \n";
    cout<<"Stor bokstav \n";
    cout<<"Liten bokstav \n";
    cout<<"Minst 8 tecken \n";
    cout<<"Minst en specialtecken \n";
    cin>>hash;
    lista = pwControl(hash);
    vector<string> errors;
    if(lista.size() > 0)
    {
        for(size_t i = 0; i < lista.size(); i++)
        {
            errors.push_back(enumConverter(lista[i]));
        }
        for(auto x: errors)
        {
            cout<<"Fel som inträffade: "<<x<<endl;
            
        }
        continue;
        
    }
    cout<<"Godkänt lösenord! \n";
      
    password = hash;
    break;
    }
    return password;
    
}

string SaltMyPassword(string& salt, string& password)
{
    password = salt + password;
    // Ändrar orginalet, lägger till salt i början
    return password;
}


string md5Hash(string& SaltedAndHashedPassword)
{
    md5(SaltedAndHashedPassword);
    return SaltedAndHashedPassword;
}


bool AddToUserFile(const string& ListOfUsers, const string& username, const string& salt, const string& hash)
{
    std::ofstream file(ListOfUsers, std::ios::app);
    if(!file.is_open())
    {
        cout<<"Failed \n";
        return false;
    }
    file<<username<<";"<<salt<<";"<<hash<<"\n";
    cout<<"Succed \n";
    return true;
}

void logIn(map<string, UserData> map)
{
    UserData user;
    cout<<"Ange användar namn: \n";
    string key;
    cin>>key;
    cout<<"Ange lösenord: \n";
    string password;
    cin>>password;
    string salt;
    string hash;
    auto it = map.find(key);
    if(it != map.end())
    {
        cout<<"User Found! \n";
        
    }
    else
    {
        cout<<"UserNotFound 404 \n";
        return;
    }
    
    salt = it->second.salt;
    hash = it->second.hash;
    // it->seoncd.hash är ju salt + password så om dessa är samma då är det korrekt pw
    string userHashedPassword = md5(salt+password);
    if(userHashedPassword == hash)
    {
        cout<<"Login Successful! Välkommen till Snabbakonton.se \n";
    }
    else
    {
        cout<<"Login Failed! \n";
    }


    

    
   

}

void LoadUsersFromFile(map <string, UserData>& users)
{
    ifstream file("ListOfUsers.txt");
    if(!file.is_open())
    {
        cout<<"Fil finns ej! \n";
        return;
    }

    string line;

    while(getline(file, line))
    //Kör så länge getline är sant
    {
        stringstream ss(line);
        string username;
        string salt;
        string hash;

        getline(ss, username, ';');
        // Kan läsas som getline, dest, source, kör till du når ';'
        getline(ss,salt,';');
        getline(ss,hash,';');

        UserData user;
        user.salt = salt;
        user.hash = hash;
        users.emplace(username, user);
    }
}


int main(void)
{
 std::map<std::string, UserData> users;
 LoadUsersFromFile(users);
 vector<passwordError> pw;
 string userID;
 string userPassword;
 string filename = "ListOfUsers.txt";
 bool flag = true;
 while(flag)
 {
    cout<<"Välkommen till SnabbaKoton.se \n";
    cout<<"Välj [1] för att logga in \n";
    cout<<"välj [2] för att skapa ett konto \n";
    int val = intControl();
    switch(val)
    {
        
        case 1:
        logIn(users);
        goNext();
        break;
        
        case 2:
        userMenu(users, userID);
        // UserID == UserID som blir ->first
        pwMenu(userPassword);
        // UserPassword == Password som retunerades med &, ->second
        string salt = createSalt();
        // Skapar salt
        SaltMyPassword(salt, userPassword);
        // Saltar skiten
        userPassword = md5(userPassword);
        // Hashar skiten
        AddToUserFile(filename,userID,salt,userPassword);
        // g++ .\password.cpp .\md5.cpp -std=c++17 -o program.exe
        UserData user;
        user.salt = salt;
        user.hash = userPassword;
        users.emplace(userID, user);
        
        
        


        goNext();
        break;
    }
 }


}