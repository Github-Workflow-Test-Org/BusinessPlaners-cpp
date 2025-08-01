#include "../Header Files/login.h"
#include "../Header Files/user_id.h"

// Function for user login or signup
void login(string username, string password, string passwordVer, bool haveAccount, char loginOrSignup, pqxx::connection* conn)
{
    cout << " ____          _                    " << endl;
    cout << "| __ )_  _ ___(_)_ __  ___ ___ ___	" << endl;
    cout << "|  _ \| | | /__| | '_ \ / _ / __/ __| " << endl;
    cout << "| |_)| |_| \__ | | | | |  __\__ \__ \  " << endl;
    cout << "|____/\__,_|___|_|_| |_|\___|___|___/ " << endl;
    cout << "|  _ \ | | __ _ _ __  _ __   ___  ___  __ " << endl;
    cout << "| |_) | |/ _` | '_  \| '_ \  / _ \ /__/ /__|" << endl;
    cout << "|  __/| | (_| | | | | | | |  __/ |  /\__ \"" << endl;
    cout << "|_|   |_|\__,_ |_| |_|_| |_|\___ |_|  |___/" << endl;
    cout << endl << endl;

    string temp = " ";

    // Display menu options
    cout << "1. Login" << endl << "2. Signup" << endl << "0. Exit" << endl;

    cin >> loginOrSignup;
    if (loginOrSignup == '1')
    {
        // User chose to login
        cout << "Enter your username: ";
        cin >> username;
        cout << endl;
        cout << "Enter your password: ";
        cin >> password;

        // Query the database for user credentials
        pqxx::work worker(*conn);
        pqxx::result res = worker.exec("SELECT id FROM users WHERE username = '" + username + "' AND password = '" + password + "'");
        if (!res.empty())
        {
            // Successful login
            pqxx::result::const_iterator row = res.begin();
            int user_id = row["id"].as<int>();
            id = user_id;
            cout << "Welcome!" << endl << endl;
        }
        else
        {
            // Invalid login credentials
            cout << "No such account!" << endl << endl;
            exit(0);
        }
    }
    else if (loginOrSignup == '2')
    {
        // User chose to signup
        cout << "Enter your username: ";
        cin >> username;

        // Check and enforce username length requirements
        unsigned int lenUser = username.length();
        if (lenUser < 6)
        {
            while (lenUser < 6)
            {
                cout << "Username must be at least 6 characters long!" << endl;
                cout << "Enter your username: ";
                cin >> password;
                unsigned int lenTest = password.length();
                lenUser = lenTest;
            }
        }
        cout << endl;
        cout << "Enter your password: ";
        cin >> password;

        // Check and enforce password length requirements
        unsigned int lenPass = password.length();
        if (lenPass < 8)
        {
            while (lenPass < 8)
            {
                cout << "Password must be at least 8 characters long!" << endl;
                cout << "Enter your password: ";
                cin >> password;
                unsigned int lenTest = password.length();
                lenPass = lenTest;
            }
        }
        cout << endl;
        cout << "Confirm your password: ";
        cin >> passwordVer;
        cout << endl;

        if (password == passwordVer)
        {
            // Passwords match, create user account
            pqxx::work worker(*conn);
            pqxx::result res = worker.exec("INSERT INTO users(username, password) VALUES('" + username + "','" + password + "')");
            worker.commit();
            system("cls");
            login(username, password, passwordVer, haveAccount, loginOrSignup, conn);
        }
        else
        {
            // Passwords do not match, provide multiple attempts
            cout << "Password doesn't match! Try again!" << endl;

            string passwordTries[5];
            for (int i = 0; i < 5; i++)
            {
                cout << "Confirm your password: ";
                cin >> passwordTries[i];

                if (5 - i - 1 == 0) {
                    cout << "Your remaining tries expired. Please come back when you remember your password!";
                    exit(0);
                }
                if (passwordTries[i] == password)
                {
                    // Successful password confirmation, create user account
                    pqxx::work worker(*conn);
                    pqxx::result res = worker.exec("INSERT INTO users(username, password) VALUES('" + username + "','" + password + "')");
                    worker.commit();
                    cout << "Account created successfully! Welcome!" << endl;
                    system("cls");
                    login(username, password, passwordVer, haveAccount, loginOrSignup, conn);
                }
                else
                {
                    // Passwords still do not match, provide feedback and remaining attempts
                    cout << "Password doesn't match! Try again!" << endl;
                    cout << "You have " << 5 - i - 1 << " more tries" << endl;
                }
            }
        }
    }
    else if (loginOrSignup == '0')
        exit(0);
}
