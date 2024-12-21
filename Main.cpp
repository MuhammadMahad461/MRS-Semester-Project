#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <stack>

using namespace std;

// Movie structure
struct Movie {
    string title;
    string category;
    float rating;
};

// User structure
struct User {
    string name;
    string email;
    string password;
    vector<string> watchedMovies; // Using vector for dynamic storage
};

// Node for linked list of watched movies
struct WatchedMovieNode {
    string title;
    WatchedMovieNode* next;
};
// Node structure for the Binary Search Tree
struct BSTNode {
    Movie movie;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Movie& m) : movie(m), left(nullptr), right(nullptr) {}
};
// Class for the Movie Recommendation System
class MovieApp {
private:
    stack<string> actionStack; // Stack to keep track of actions for undo
    queue<string> watchLaterQueue; // Queue for movies to watch later
    vector<User> users; // Array for storing users
    vector<Movie> movies; // Array for storing movies
    User* currentUser  = nullptr; // Pointer to the currently logged-in user
    WatchedMovieNode* watchedMoviesHead = nullptr; // Head of the linked list for watched movies
BSTNode* bstRoot; // Root of the BST for movies
    // Load users from file
   // Load users from file
void loadUsers() {
    ifstream file("users.txt");
    if (file.is_open()) {
        User user;
        while (getline(file, user.name, ' ') && 
               getline(file, user.email, ' ') && 
               getline(file, user.password)) {
            users.push_back(user);
        }
        file.close();
    }
}

    // Save users to file
    void saveUsers() {
        ofstream file("users.txt");
        for (const auto& user : users) {
            file << user.name << " " << user.email << " " << user.password << endl;
        }
        file.close();
    }

    // Load movies from file
  void loadMovies() {
    ifstream file("movies.txt");
    if (file.is_open()) {
        Movie movie;
        while (file >> movie.title >> movie.category >> movie.rating) {
            movies.push_back(movie);
            addMovieToBST(movie); // Add movie to BST
        }
        file.close();
    }
}
// Load watched movies from a user-specific file
void loadWatchedMovies() {
    if (currentUser ) {
        ifstream file(currentUser ->email + "_watched_movies.txt");
        if (file.is_open()) {
            string title;
            while (getline(file, title)) {
                currentUser ->watchedMovies.push_back(title);
                addWatchedMovie(title);
            }
            file.close();
        }
    }
}
    // Save movies to file
    void saveMovies() {
        ofstream file("movies.txt");
        for (const auto& movie : movies) {
            file << movie.title << " " << movie.category << " " << movie.rating << endl;
        }
        file.close();
    }

    // Add a watched movie to the linked list
    void addWatchedMovie(const string& title) {
        WatchedMovieNode* newNode = new WatchedMovieNode{title, nullptr};
        if (!watchedMoviesHead) {
            watchedMoviesHead = newNode;
        } else {
            WatchedMovieNode* temp = watchedMoviesHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Display watched movies
    void displayWatchedMovies() {
        WatchedMovieNode* temp = watchedMoviesHead;
        cout << "Watched Movies: ";
        while (temp) {
            cout << temp->title << " ";
            temp = temp->next;
        }
        cout << endl;
    }

   
     // Recommend movies based on watched movies
void recommendMovies() {
    if (currentUser ->watchedMovies.empty()) {
        cout << "No watched movies to base recommendations on." << endl;
        return;
    }

    unordered_map<string, vector<Movie>> categoryMovies;
    for (const auto& movie : movies) {
        categoryMovies[movie.category].push_back(movie);
    }

    unordered_map<string, int> categoryCount;
    for (const auto& title : currentUser ->watchedMovies) {
        for (const auto& movie : movies) {
            if (movie.title == title) {
                categoryCount[movie.category]++;
            }
        }
    }

    for (const auto& pair : categoryCount) {
        const string& category = pair.first;
        cout << "Recommended movies in category (" << category << "):" << endl;

        // Shuffle the movies in the category
        vector<Movie>& moviesInCategory = categoryMovies[category];
        random_shuffle(moviesInCategory.begin(), moviesInCategory.end());

        // Display up to 5 random movies from the category
        int count = 0;
        for (const auto& movie : moviesInCategory) {
            if (count < 5) {
                cout << "Title: " << movie.title << ", Rating: " << movie.rating << endl;
                count++;
            } else {
                break;
            }
        }
    }
}
    // Search for a movie by title or category
    void searchMovies() {
        string query;
        cout << "Enter movie title or category to search: ";        cin >> query;
        cout << "Search Results:" << endl;
        bool found = false;
        for (const auto& movie : movies) {
            if (movie.title.find(query) != string::npos || movie.category.find(query) != string::npos) {
                cout << "Title: " << movie.title << ", Category: " << movie.category << ", Rating: " << movie.rating << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No movies found matching your query." << endl;
        }
    }

    // Sort movies by title
    void sortMoviesByTitle() {
        sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
            return a.title < b.title;
        });
        cout << "Movies sorted by title." << endl;
    }

    // Sort movies by rating
    void sortMoviesByRating() {
        sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
            return a.rating > b.rating; // Sort in descending order
        });
        cout << "Movies sorted by rating." << endl;
    }
// Sort movies by category using Bubble Sort
void sortMoviesByCategory() {
    for (size_t i = 0; i < movies.size() - 1; ++i) {
        for (size_t j = 0; j < movies.size() - i - 1; ++j) {
            if (movies[j].category > movies[j + 1].category) {
                swap(movies[j], movies[j + 1]);
            }
        }
    }
    cout << "Movies sorted by category." << endl;
}
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
  void markMovieAsWatched() {
    string title;
    cout << "Enter movie title to mark as watched: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, title); // Use getline to allow spaces in titles

    for (const auto& movie : movies) {
        if (movie.title == title) {
            currentUser ->watchedMovies.push_back(title);
            addWatchedMovie(title);
            actionStack.push("Marked as watched: " + title); // Push action onto stack
            saveWatchedMovies(); // Save watched movies after marking
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
    // Preview 10 random movies from each category
void previewMovies() {
    unordered_map<string, vector<Movie>> categoryMovies;
    
    // Group movies by category
    for (const auto& movie : movies) {
        categoryMovies[movie.category].push_back(movie);
    }

    // Display 10 random movies from each category
    for (const auto& pair : categoryMovies) {
        const string& category = pair.first;
        cout << "Preview of movies in category (" << category << "):" << endl;

        // Shuffle the movies in the category
        vector<Movie>& moviesInCategory = categoryMovies[category];
        random_shuffle(moviesInCategory.begin(), moviesInCategory.end());

        // Display up to 10 random movies from the category
        int count = 0;
        for (const auto& movie : moviesInCategory) {
            if (count < 10) {
                cout << "Title: " << movie.title << ", Rating: " << movie.rating << endl;
                count++;
            } else {
                break;
            }
        }
        cout << endl; // Add a newline for better readability
    }
}
// View the user's watch history
void viewWatchHistory() {
    if (currentUser ->watchedMovies.empty()) {
        cout << "No watched movies in your history." << endl;
        return;
    }

    cout << "Your Watch History:" << endl;
    for (const auto& title : currentUser ->watchedMovies) {
        cout << title << endl;
    }
}

// Delete the user's watch history
void deleteWatchHistory() {
    currentUser ->watchedMovies.clear(); // Clear the vector
    saveWatchedMovies(); // Save the empty history to the file
    cout << "Your watch history has been deleted." << endl;
}
  void userMenu() {
    int choice;
    while (true) {
        cout << "\nUser  Menu:\n";
        cout << "1. Mark Movie as Watched\n";
        cout << "2. Recommend Movies\n";
        cout << "3. Watch Random Movie\n";
        cout << "4. View All Movies\n";
        cout << "5. Search Movies\n";
        cout << "6. View Watch History\n";
        cout << "7. Delete Watch History\n";
        cout << "8. Preview Movies\n";
        cout << "9. Undo Last Action\n"; // Option for undo
        cout << "10. Add Movie to Watch Later\n"; // Option for watch later
        cout << "11. Watch Next Movie\n"; // Option for watching next
        cout << "12. Logout\n"; // Updated option number
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                markMovieAsWatched();
                break;
            case 2:
                recommendMovies();
                break;
            case 3:
                watchRandomMovie();
                break;
            case 4:
                viewAllMovies();
                break;
            case 5:
                searchMovies();
                break;
            case 6:
                viewWatchHistory();
                break;
            case 7:
                deleteWatchHistory();
                break;
            case 8:
                previewMovies();
                break;
            case 9:
                undoLastAction(); // Call the undo function
                break;
            case 10: {
                string title;
                cout << "Enter movie title to add to watch later: ";
                cin.ignore(); // Clear the input buffer
                getline(cin, title);
                addToWatchLater(title);
                saveWatchLaterQueue(); // Save the queue after adding
                break;
            }
            case 11:
                watchNext();
                break;
            case 12:
                saveWatchLaterQueue(); // Save the queue before logout currentUser  = nullptr; // Logout
                cout << "Logged out successfully!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
   void mainMenu() {
    int choice;
    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n"; // Removed the option to view all movies
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (login()) {
                    userMenu(); // Show user options after login
                }
                break;
            case 3: {
                string adminPassword = "admin123"; // Simple admin password
                string inputPassword;
                cout << "Enter admin password: ";
                cin >> inputPassword;
                if (inputPassword == adminPassword) {
                    adminMenu(); // Show admin options
                } else {
                    cout << "Invalid admin password!" << endl;
                }
                break;
            }
            case 4:
                return; // Exit the program
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
};

// Main function to handle user interactions
int main() {
    MovieApp app;
    app.mainMenu(); // Start the main menu
    return 0;
}
