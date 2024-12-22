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
        cout << "Random Movie: " << movies[randomIndex].title << ", Category: " << movies[randomIndex].category << ", Rating: " << movies[randomIndex].rating << endl;
    }
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
        cout << "9. Undo Last Action\n";
        cout << "10. Add Movie to Watch Later\n";
        cout << "11. Watch Next Movie\n";
        cout << "12. View Watch Later Queue\n"; // New option to view watch later queue
        cout << "13. Logout\n"; // Updated option number
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
                undoLastAction();
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
            case 12: // New case for viewing watch later queue
                displayWatchLaterQueue();
                break;
            case 13:
                saveWatchLaterQueue(); // Save the queue before logout
                currentUser  = nullptr; // Logout
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
