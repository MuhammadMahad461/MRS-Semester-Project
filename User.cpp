class User {
public:
    string name;
    string email;
    string password;

    User(string n, string e, string p) : name(n), email(e), password(p) {}
};
 void saveUsers() {
        ofstream outfile(userFile);
        for (const auto& user : users) {
            outfile << user.name << " " << user.email << " " << user.password << endl;
        }
        outfile.close();
    }
 void signUp() {
        string name, email, password;
        cout << "Enter full name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);

        for (const auto& user : users) {
            if (user.email == email) {
                cout << "Email already exists!" << endl;
                return;
            }
        }

        users.emplace_back(name, email, password);
        cout << "Sign-up successful!" << endl;
    }

    // Login functionality
    bool login(string& loggedInEmail, bool& isAdmin) {
        string email, password;
        cout << "Enter email: ";
        cin.ignore();
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);

        if (email == adminEmail && password == adminPassword) {
            isAdmin = true;
            loggedInEmail = email;
            cout << "Admin login successful!" << endl;
            return true;
        }

        for (const auto& user : users) {
            if (user.email == email && user.password == password) {
                isAdmin = false;
                loggedInEmail = email;
                cout << "Login successful!" << endl;
                return true;
            }
        }

        cout << "Invalid email or password!" << endl;
        return false;
    } void showUserMovies() {
        vector<string> categories = {"Horror", "Comedy", "Romance", "Action"};
        
        // Loop through categories
        for (const auto& category : categories) {
            // Get two random movies from each category
            auto randomMovies = getRandomMovies(category, 2);
            
            // Display category header
            cout << "\nCategory: " << category << endl;
            
            // Display the selected random movies
            for (const auto& movie : randomMovies) {
                cout << "Title: " << movie.title << ", Category: " << movie.category << ", Rating: " << movie.rating << endl;
            }
        }
    } void userMenu(const string& loggedInEmail) {
        while (true) {
            int choice;
            cout << "1. Show Random Movies\n2. View Movies\n3. Logout\nChoose an option: ";
            cin >> choice;

            if (choice == 1) {
                showUserMovies();
            } else if (choice == 2) {
                viewAdminMovies();
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice!" << endl;
            }
        }
    }