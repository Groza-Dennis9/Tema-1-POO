#include <iostream>
#include <cstring>
using namespace std;

class CatCard {
private:
    int tier;
    bool isRare;
    char gender;
    char* breedName;
    static int totalCatsCreated;

    void deepCopy (const char* name);
    const char* getBreedByTier (int t) const ;

public:
    //Constructors
    CatCard(int t, bool rare, char g, const char* name = nullptr);
    CatCard(const CatCard& other);
    CatCard();
    //Destructor
    ~CatCard();

    //Setters and Getters
    void setTier(int t);
    int getTier() const;
    const char* getBreed() const;

    //Operators overload
    CatCard& operator=(const CatCard& other);
    friend ostream& operator<<(ostream& os, const CatCard& c);
    friend istream& operator>>(istream& is, CatCard& c);
};
int CatCard::totalCatsCreated = 0;

CatCard::CatCard(int t, bool rare, char g, const char *name) : tier(t), isRare(rare), gender(g) {
    if (name)
        deepCopy(name);
    else
        deepCopy(getBreedByTier(t));
    totalCatsCreated++;
}
CatCard::CatCard(const CatCard &other) : tier(other.tier), isRare(other.isRare), gender(other.gender) {
    deepCopy(other.breedName);
}
CatCard::CatCard() {
    tier = 0;
    isRare = false;
    gender = '\0';
    breedName = nullptr;
}
CatCard::~CatCard() {
    delete[] breedName;
}

void CatCard::deepCopy(const char* name) {
    if (name) {
        breedName = new char[strlen(name) + 1];
        strcpy(breedName, name);
    }
    else
        breedName = nullptr;
}
const char* CatCard::getBreedByTier (int t) const {
    switch (t) {
        case 1: return "Kitten";
        case 2: return "Tabby";
        case 3: return "Bengal";
        case 4: return "Persian";
        case 5: return "MaineCoon";
        case 6: return "Sphinx";
        default: return "LegendaryCat";
    }
}

void CatCard::setTier(int t) {
    tier = t;
    delete[] breedName;
    deepCopy(getBreedByTier(t));
}
int CatCard::getTier() const {
    return tier;
}
const char* CatCard::getBreed() const {
    return breedName;
}

CatCard& CatCard::operator= (const CatCard& other) {
    if (this != &other) {
        delete[] breedName;
        tier = other.tier;
        isRare = other.isRare;
        gender = other.gender;
        deepCopy(other.breedName);
    }
    return *this;
}
ostream& operator<<(ostream& os, const CatCard& c) {
    os << "[ "<<(c.breedName ? c.breedName : "Unknown") << " ~ T" << c.tier <<" ~ "<< c.gender << " ]";
    return os;
}
istream& operator>>(istream& is, CatCard& c) {
    //cout<<"Name: ";
    char buffer[100];
    is >> buffer;
    delete[] c.breedName;
    c.deepCopy(buffer);

    //cout<<"Tier: ";
    is >> c.tier;
    return is;
}

/*---------------------------------------------------------------------------------------*/

class ShelterSpot {
private:
    CatCard* card;
    float cleanliness;
    int* logHistory;
    const int spotID;

public:
    //Constructor
    ShelterSpot(int id);
    ShelterSpot(const ShelterSpot& other);
    //Destructor
    ~ShelterSpot();

    //Getters
    CatCard* getCard() const;
    float getCleanliness() const;

    //Functions
    bool isEmpty() const;
    void occupy(const CatCard& c);
    void evict();
    void deteriorate();
    void clean();

    //Operators overload
    ShelterSpot& operator=(const ShelterSpot& other);
    friend ostream& operator<<(ostream& os, const ShelterSpot& s);
};

ShelterSpot::ShelterSpot(int id) : card(nullptr), cleanliness(100), spotID(id){
    logHistory = new int[3]{0, 0, 0};
}
ShelterSpot::ShelterSpot(const ShelterSpot &other) : spotID(other.spotID) {
    card = other.card ? new CatCard(*other.card) : nullptr;
    logHistory = new int[3];
    for (int i = 0; i < 3; i++)
        logHistory[i] = other.logHistory[i];
    cleanliness = other.cleanliness;
}
ShelterSpot::~ShelterSpot() {
    delete card;
    delete[] logHistory;
}

CatCard* ShelterSpot::getCard() const {
    return card;
}
float ShelterSpot::getCleanliness() const {
    return cleanliness;
}

bool ShelterSpot::isEmpty() const {
    return card == nullptr;
}
void ShelterSpot::occupy(const CatCard& c) {
    delete card;
    card = new CatCard(c);
    logHistory[0]++;
}
void ShelterSpot::evict() {
    delete card;
    card = nullptr;
}
void ShelterSpot::deteriorate() {
    if (card) {
        cleanliness -= 15.0f;
        if (cleanliness < 0) cleanliness = 0;
    }
}
void ShelterSpot::clean() {
    cleanliness = 100.0f;
    logHistory[2]++;
}

ShelterSpot& ShelterSpot:: operator=(const ShelterSpot& other) {
        if(this != &other) {
            delete card;
            card = other.card ? new CatCard(*other.card) : nullptr;
            cleanliness = other.cleanliness;
            for(int i=0; i<3; ++i) logHistory[i] = other.logHistory[i];
        }
        return *this;
    }
ostream& operator<<(ostream& os, const ShelterSpot& s) {
    os << "Slot " << s.spotID << ": ";
    if (s.card) os << *s.card; else os << "Empty";
    os << " | Clean: " << s.cleanliness << "% (History: " << s.logHistory[2] << " cleans)";
    return os;
}

/*---------------------------------------------------------------------------------------*/

class Economy {
    //handling the point system
private:
    double balance;
    double multiplier;
    //only two variables for now

public:
    //Constructor
    Economy(double b, double m);
    //need copy constructor and destructor

    //Getter
    double getBalance() const;

    //Function
    void addFunds(double amount);

    //Operator overload
    friend ostream& operator<<(ostream& os, const Economy& e);
};

Economy::Economy(double b, double m) : balance(b), multiplier(m) {}

double Economy::getBalance() const {
    return balance;
}
void Economy::addFunds(double amount) {
    balance += (amount * multiplier);
}

ostream& operator<<(ostream& os, const Economy& e) {
    os << "Purr-Points: " << e.balance;
    return os;
}

/*---------------------------------------------------------------------------------------*/

class GameEngine {
    //running the game
private:
    ShelterSpot* spots[6];
    Economy stats;
    bool isRunning;
    int difficulty;

    bool canMergeAny() const;
    bool isShelterFull() const;
public:
    //Constructors
    GameEngine();
    GameEngine(int diff);
    GameEngine(const GameEngine& o);
    //Destructor
    ~GameEngine();

    //Functions
    void spawnCat();
    void attemptMerge(int a, int b);
    void cleanSlot(int id);

    void play();

    //Operator overload
    GameEngine& operator=(const GameEngine& o);
    friend ostream& operator<<(ostream& os, const GameEngine& ge);
    friend istream& operator>>(istream& is, GameEngine& ge);
};

GameEngine::GameEngine() : stats(10.0, 1.0), isRunning(true), difficulty(0) {
    for (int i = 0; i < 6; ++i) spots[i] = new ShelterSpot(i);
}
GameEngine::GameEngine(int diff) : stats(0.0, 1.5), isRunning(true), difficulty(diff) {
    for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(i);
}
GameEngine::GameEngine(const GameEngine& o) : stats(o.stats), isRunning(o.isRunning), difficulty(o.difficulty) {
    for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(*o.spots[i]);
}
GameEngine::~GameEngine() {
    for (int i = 0; i < 6; ++i) delete spots[i];
}

bool GameEngine::canMergeAny() const {
    for (int i = 0; i < 6; ++i) {
        for (int j = i + 1; j < 6; ++j) {
            if (!spots[i]->isEmpty() && !spots[j]->isEmpty()) {
                if (spots[i]->getCard()->getTier() == spots[j]->getCard()->getTier())
                    return true;
            }
        }
    }
    return false;
}
bool GameEngine::isShelterFull() const {
    for (int i = 0; i < 6; ++i)
        if (spots[i]->isEmpty())
            return false;
    return true;
}

void GameEngine::spawnCat() {
    for (int i = 0; i < 6; ++i) {
        if (spots[i]->isEmpty() && spots[i]->getCleanliness() > 20.0f) {
            spots[i]->occupy(CatCard(1, false, 'F'));
            for(int j=0; j<6; j++)
                spots[j]->deteriorate();
            return;
        }
    }
    std::cout << "Cannot rescue! Shelter full or too dirty.\n";
}
void GameEngine::attemptMerge(int a, int b) {
    if (a >= 0 && a < 6 && b >= 0 && b < 6 && a != b && !spots[a]->isEmpty() && !spots[b]->isEmpty()) {
        if (spots[a]->getCard()->getTier() == spots[b]->getCard()->getTier()) {
            int nextTier = spots[a]->getCard()->getTier() + 1;
            spots[a]->occupy(CatCard(nextTier, false, 'M'));
            spots[b]->evict();
            stats.addFunds(10.0);
            for(int j=0; j<6; j++) spots[j]->deteriorate();
        }
    }
    else
        std::cout << "Merge invalid!\n";
}
void GameEngine::cleanSlot(int id) {
    if (stats.getBalance() >= 5.0) {
        stats.addFunds(-5.0);
        spots[id]->clean();
    } else {
        std::cout << "Not enough points!\n";
    }
}

void GameEngine::play() {
    while (isRunning)
    {
        // std::cout << "================================" << std::endl;
        // std::cout << "      CAT SHELTER MANAGER       " << std::endl;
        // std::cout << "================================" << std::endl;
        // std::cout << stats << std::endl;
        //idea: clear the screen for each new load

        std::cout << "\n" << stats << "\n";
        for (int i = 0; i < 6; ++i)
            std::cout << *spots[i] << "\n";
        if (isShelterFull() && !canMergeAny()) {
            std::cout << "\n!!! GAME OVER !!! No more moves possible.\n";
            isRunning = false;
        }
        std::cout << "1:Rescue | 2:Merge | 3:Clean (5pts) | 4:Exit\nChoice: ";
        int choice; std::cin >> choice;

        switch (choice) {
            case 1: spawnCat();
                break;
            case 2:
            { int a, b;
                std::cout << "Enter IDs to merge: ";
                std::cin >> a >> b;
                attemptMerge(a, b);
            }
                break;
            case 3:
            { int id;
                std::cout << "Enter the ID to clean: ";
                std::cin >> id;
                cleanSlot(id);
            }
                break;
            default: isRunning = false;
        }
        for (int i=0; i<100; i++) {
            cout<<endl;
        }
    }

}

GameEngine& GameEngine::operator=(const GameEngine& o) {
    if(this != &o) {
        for(int i=0; i<6; ++i) delete spots[i];
        stats = o.stats; isRunning = o.isRunning; difficulty = o.difficulty;
        for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(*o.spots[i]);
    }
    return *this;
}
ostream& operator<<(ostream& os, const GameEngine& ge) {
    return os << ge.difficulty;
}
istream& operator>>(istream& is, GameEngine& ge) {
    return is >> ge.difficulty;
}

int main() {
    GameEngine game;
    game.play();
    return 0;
}