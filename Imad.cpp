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
void displayWatchLaterQueue() {
    if (watchLaterQueue.empty()) {
        cout << "No movies in the watch later queue." << endl;
        return;
    }

    cout << "Movies in Watch Later Queue:" << endl;
    queue<string> tempQueue = watchLaterQueue; // Create a temporary queue to display
    while (!tempQueue.empty()) {
        cout << tempQueue.front() << endl; // Print the front movie
        tempQueue.pop(); // Remove it from the temporary queue
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
void removeMovie() {
    string title;
    cout << "Enter movie title to remove: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, title); // Use getline to allow spaces in titles

    auto it = remove_if(movies.begin(), movies.end(), [&title](const Movie& movie) {
        return movie.title == title;
    });

    if (it != movies.end()) {
        movies.erase(it, movies.end());
        cout << "Movie removed successfully!" << endl;
    } else {
        cout << "Movie not found!" << endl;
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

// In-order traversal to display movies in sorted order
void inOrderTraversal(BSTNode* root) {
    if (root != nullptr) {
        inOrderTraversal(root->left);
        cout << "Title: " << root->movie.title << ", Category: " << root->movie.category << ", Rating: " << root->movie.rating << endl;
        inOrderTraversal(root->right);
    }
}

// Display all movies in sorted order
void displayMoviesInOrder() {
    cout << "Movies in sorted order:" << endl;
    inOrderTraversal(bstRoot);
}
    void viewAllMovies() {
        cout << "Available Movies:" << endl;
        for (const auto& movie : movies) {
            cout << "Title: " << movie.title << ", Category: " << movie.category << ", Rating: " << movie.rating << endl;
        }
    }
// Save watched movies to a user-specific file
void saveWatchedMovies() {
    if (currentUser ) {
        ofstream file(currentUser ->email + "_watched_movies.txt");
        for (const auto& title : currentUser ->watchedMovies) {
            file << title << endl;
        }
        file.close();
    }
}
void removeFromWatchLaterQueue(const string& title) {
    queue<string> tempQueue; // Temporary queue to hold movies
    bool found = false;

    // Iterate through the watch later queue
    while (!watchLaterQueue.empty()) {
        string currentMovie = watchLaterQueue.front();
        watchLaterQueue.pop();

        // If the current movie is the one to be removed, set found to true
        if (currentMovie == title) {
            found = true;
            cout << "Removed from watch later queue: " << title << endl;
            continue; // Skip adding this movie to the temporary queue
        }
        // Add the current movie to the temporary queue
        tempQueue.push(currentMovie);
    }

    // Restore the original queue without the removed movie
    watchLaterQueue = tempQueue;

    // If the movie was not found in the queue
    if (!found) {
        cout << "Movie not found in watch later queue!" << endl;
    }
}
 void markMovieAsWatched() {
    string title;
    cout << "Enter movie title to mark as watched: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, title); // Use getline to allow spaces in titles

    // Check if the movie exists in the movies list
    for (const auto& movie : movies) {
        if (movie.title == title) {
            currentUser ->watchedMovies.push_back(title);
            addWatchedMovie(title);
            actionStack.push("Marked as watched: " + title); // Push action onto stack
            saveWatchedMovies(); // Save watched movies after marking

            // Remove the movie from the watch later queue if it exists
            removeFromWatchLaterQueue(title);

            cout << "Movie marked as watched!" << endl;
            return;
        }
    }
    cout << "Movie not found!" << endl;
}
    void watchRandomMovie() {
        if (movies.empty()) {
            cout << "No movies available to watch." << endl;
            return;
        }
        int randomIndex = rand() % movies.size();
        cout << "Random Movie: " << movies[randomIndex].title << ", Category: " << movies[randomIndex].category << ", Rating: " << movies[randomIndex].rating << endl;
    }
