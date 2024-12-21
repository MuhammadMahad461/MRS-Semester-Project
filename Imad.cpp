 // Admin menu for managing users and movies
// Admin menu for managing users and movies

void adminMenu() {
    int choice;
    while (true) {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Movie\n";
        cout << "2. View All Movies\n";
        cout << "3. Sort Movies by Title\n";
        cout << "4. Sort Movies by Rating\n";
        cout << "5. Sort Movies by Category\n"; // New option
        cout << "6. View All Users\n";
        cout << "7. Logout\n";
        cout<<"8. Display Movies in Order"<<endl; // New option
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Movie newMovie;
                cout << "Enter movie title: ";
                cin.ignore(); // Clear the input buffer
                getline(cin, newMovie.title); // Use getline to allow spaces in titles
                cout << "Enter movie category: ";
                getline(cin, newMovie.category); // Use getline for category
                cout << "Enter movie rating: ";
                cin >> newMovie.rating;
                movies.push_back(newMovie);
                cout << "Movie added successfully!" << endl;
                break;
            }
            case 2:
                viewAllMovies();
                break;
            case 3:
                sortMoviesByTitle();
                break;
            case 4:
                sortMoviesByRating();
                break;
            case 5:
                sortMoviesByCategory(); // Call the new sorting function
                break;
            case 6:
                cout << "Registered Users:" << endl;
                for (const auto& user : users) {
                    cout << "Name: " << user.name << ", Email: " << user.email << endl;
                }
                break;
            case 7:
                return; // Logout
                case 8: // Add this case for displaying movies in order
    displayMoviesInOrder();
    break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

public:
   MovieApp() : bstRoot(nullptr) {
    loadUsers();
    loadMovies();
    srand(static_cast<unsigned int>(time(0))); // Seed for randomization
}

    ~MovieApp() {
        saveUsers();
        saveMovies();
        // Free linked list memory
        while (watchedMoviesHead) {
            WatchedMovieNode* temp = watchedMoviesHead;
            watchedMoviesHead = watchedMoviesHead->next;
            delete temp;
        }
    }
    void addToWatchLater(const string& title) {
    watchLaterQueue.push(title);
    cout << "Added to watch later: " << title << endl;
}
void undoLastAction() {
    if (actionStack.empty()) {
        cout << "No actions to undo." << endl;
        return;
    }
    
    string lastAction = actionStack.top();
    actionStack.pop();

    // Check if the last action was marking a movie as watched
    if (lastAction.find("Marked as watched: ") != string::npos) {
        string title = lastAction.substr(lastAction.find(": ") + 2);
        
        // Remove the movie from the watched list
        auto it = find(currentUser ->watchedMovies.begin(), currentUser ->watchedMovies.end(), title);
        if (it != currentUser ->watchedMovies.end()) {
            currentUser ->watchedMovies.erase(it);
            saveWatchedMovies(); // Save the updated watched movies list
            cout << "Undid action: " << lastAction << endl;
        } else {
            cout << "Movie not found in watched list!" << endl;
        }
    } else {
        cout << "No undo action available for: " << lastAction << endl;
    }
}

void watchNext() {
    if (watchLaterQueue.empty()) {
        cout << "No movies in the watch later queue." << endl;
        return;
    }
    string nextMovie = watchLaterQueue.front();
    watchLaterQueue.pop();
    cout << "Now watching: " << nextMovie << endl;
}
void saveWatchLaterQueue() {
    ofstream file(currentUser ->email + "_watch_later.txt");
    if (file.is_open()) {
        queue<string> tempQueue = watchLaterQueue; // Create a temporary queue
        while (!tempQueue.empty()) {
            file << tempQueue.front() << endl; // Write each movie to the file
            tempQueue.pop();
        }
        file.close();
    }
}
void loadWatchLaterQueue() {
    if (currentUser ) {
        ifstream file(currentUser ->email + "_watch_later.txt");
        if (file.is_open()) {
            string title;
            while (getline(file, title)) {
                watchLaterQueue.push(title); // Add each movie to the queue
            }
            file.close();
        }
    }
}

   void signUp() {
    User newUser ;
    cout << "Enter name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, newUser .name); // Use getline to allow spaces in names
    cout << "Enter email: ";
    cin >> newUser .email;
    cout << "Enter password: ";
    cin >> newUser .password;
    users.push_back(newUser );
    saveUsers(); // Save users after adding a new user
    cout << "User  registered successfully!" << endl;
}

 bool login() {
    string email, password;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    for (auto& user : users) {
        if (user.email == email && user.password == password) {
            currentUser  = &user;
            loadWatchedMovies(); // Load watched movies after login
            loadWatchLaterQueue(); // Load watch later queue after login
            cout << "Login successful!" << endl;
            return true;
        }
    }
    cout << "Invalid credentials!" << endl;
    return false;
}
// Insert a movie into the BST
void insertMovieBST(BSTNode*& root, const Movie& movie) {
    if (root == nullptr) {
        root = new BSTNode(movie);
    } else if (movie.title < root->movie.title) {
        insertMovieBST(root->left, movie);
    } else {
        insertMovieBST(root->right, movie);
    }
}

// Add a movie to the BST
void addMovieToBST(const Movie& movie) {
    insertMovieBST(bstRoot, movie);
}

// Search for a movie by title in the BST
BSTNode* searchMovieBST(BSTNode* root, const string& title) {
    if (root == nullptr || root->movie.title == title) {
        return root;
    }
    if (title < root->movie.title) {
        return searchMovieBST(root->left, title);
    }
    return searchMovieBST(root->right, title);
}

// Search for a movie in the BST
void searchMovieInBST(const string& title) {
    BSTNode* result = searchMovieBST(bstRoot, title);
    if (result) {
        cout << "Movie found: " << result->movie.title << ", Category: " << result->movie.category << ", Rating: " << result->movie.rating << endl;
    } else {
        cout << "Movie not found!" << endl;
    }
}
