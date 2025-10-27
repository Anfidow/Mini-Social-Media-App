#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>

using namespace std;

// Base class for entities with IDs
class Entity {
protected:
    int id;
    static int nextId;
public:
    Entity() : id(nextId++) {}
    int getId() const { return id; }
};

int Entity::nextId = 1;

// Post class
class Post : public Entity {
private:
    string content;
    int authorId;
    time_t timestamp;
public:
    Post(string c, int aId) : content(c), authorId(aId), timestamp(time(NULL)) {}
    string getContent() const { return content; }
    int getAuthorId() const { return authorId; }
    string getTimestamp() const {
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&timestamp));
        return string(buf);
    }
    virtual void display() const {
        cout << "Post by User " << authorId << ": " << content << " (" << getTimestamp() << ")" << endl;
    }
};

// User class
class User : public Entity {
private:
    string name;
    vector<Post*> posts;
    vector<int> followers;
    vector<int> following;
public:
    User(string n) : name(n) {}
    string getName() const { return name; }
    void addPost(Post* p) { posts.push_back(p); }
    void follow(int userId) { following.push_back(userId); }
    void addFollower(int userId) { followers.push_back(userId); }
    vector<Post*> getPosts() const { return posts; }
    vector<int> getFollowing() const { return following; }
    void displayProfile() const {
        cout << "User: " << name << " (ID: " << id << ")" << endl;
        cout << "Followers: " << followers.size() << ", Following: " << following.size() << endl;
        cout << "Posts:" << endl;
        for (auto p : posts) p->display();
    }
};

// Social Media App class
class SocialMediaApp {
private:
    unordered_map<int, User*> users;
public:
    ~SocialMediaApp() {
        for (auto& pair : users) delete pair.second;
    }
    void createUser(string name) {
        User* u = new User(name);
        users[u->getId()] = u;
        cout << "User created: " << name << " (ID: " << u->getId() << ")" << endl;
    }
    void postMessage(int userId, string content) {
        if (users.find(userId) != users.end()) {
            Post* p = new Post(content, userId);
            users[userId]->addPost(p);
            cout << "Post added." << endl;
        } else {
            cout << "User not found." << endl;
        }
    }
    void followUser(int followerId, int followeeId) {
        if (users.find(followerId) != users.end() && users.find(followeeId) != users.end()) {
            users[followerId]->follow(followeeId);
            users[followeeId]->addFollower(followerId);
            cout << "Followed." << endl;
        } else {
            cout << "User(s) not found." << endl;
        }
    }
    void viewFeed(int userId) {
        if (users.find(userId) == users.end()) {
            cout << "User not found." << endl;
            return;
        }
        cout << "Feed for User " << userId << ":" << endl;
        vector<int> following = users[userId]->getFollowing();
        for (int fId : following) {
            if (users.find(fId) != users.end()) {
                vector<Post*> posts = users[fId]->getPosts();
                for (auto p : posts) p->display();
            }
        }
    }
    void displayUser(int userId) {
        if (users.find(userId) != users.end()) {
            users[userId]->displayProfile();
        } else {
            cout << "User not found." << endl;
        }
    }
};

// Main function with a simple menu
int main() {
    SocialMediaApp app;
    int choice, userId, followeeId;
    string name, content;
    
    while (true) {
        cout << "\nMini Social Media App\n";
        cout << "1. Create User\n2. Post Message\n3. Follow User\n4. View Feed\n5. View Profile\n6. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Enter name: ";
                cin >> name;
                app.createUser(name);
                break;
            case 2:
                cout << "Enter user ID: ";
                cin >> userId;
                cout << "Enter message: ";
                cin.ignore();
                getline(cin, content);
                app.postMessage(userId, content);
                break;
            case 3:
                cout << "Enter follower ID: ";
                cin >> userId;
                cout << "Enter followee ID: ";
                cin >> followeeId;
                app.followUser(userId, followeeId);
                break;
            case 4:
                cout << "Enter user ID: ";
                cin >> userId;
                app.viewFeed(userId);
                break;
            case 5:
                cout << "Enter user ID: ";
                cin >> userId;
                app.displayUser(userId);
                break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice." << endl;
        }
    }
    return 0;
}
