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
// Admin menu for managing users and movies
void adminMenu() {
    int choice;
    while (true) {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Movie\n";
        cout << "2. View All Movies\n";
        cout << "3. Sort Movies by Title\n";
        cout << "4. Sort Movies by Rating\n";
        cout << "5. Sort Movies by Category\n"; 
        cout << "6. Remove Movie\n"; // New option for removing a movie
        cout << "7. View All Users\n";
        cout << "8. Display Movies in Order\n";
        cout << "9. Search Movies\n"; // New option for searching movies
        cout << "10. Logout\n"; 
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
                removeMovie(); // Call the remove movie function
                break;
            case 7:
                cout << "Registered Users:" << endl;
                for (const auto& user : users) {
                    cout << "Name: " << user.name << ", Email: " << user.email << endl;
                }
                break;
            case 8: // Add this case for displaying movies in order
               displayMoviesInOrder();
                break;
            case 9: // New case for searching movies
                searchMovies(); // Call the search function for admin
                break;
            case 10: //Log Out
                 return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
