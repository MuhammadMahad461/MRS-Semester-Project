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
