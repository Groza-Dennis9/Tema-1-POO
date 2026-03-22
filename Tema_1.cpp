#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

class CatCard {
private:
    int tier;
    bool isRare;
    char gender;
    char* breedName;
    static int totalCatsCreated;
    const int catID;

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

    //Operator overloads
    CatCard& operator=(const CatCard& other);
    friend ostream& operator<<(ostream& os, const CatCard& c);
    friend istream& operator>>(istream& is, CatCard& c);
};
int CatCard::totalCatsCreated = 0;

//Constructors
CatCard::CatCard(int t, bool rare, char g, const char *name) : tier(t), isRare(rare), gender(g), breedName(nullptr), catID(++totalCatsCreated) {
    if (name)
        deepCopy(name);
    else
        deepCopy(getBreedByTier(t));
    totalCatsCreated++;
}
CatCard::CatCard(const CatCard &other) : tier(other.tier), isRare(other.isRare), gender(other.gender), catID(other.catID), breedName(nullptr) {
    deepCopy(other.breedName);
}
CatCard::CatCard() : catID(++totalCatsCreated) {
    tier = 0;
    isRare = false;
    gender = '\0';
    breedName = nullptr;
}
//Destructors
CatCard::~CatCard() {
    delete[] breedName;
}

//Functions (private)
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
        default: return "Unknown";
    }
}

//Setters and Getters
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

//Operator overloads
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
    char buffer[100];
    is >> buffer;
    delete[] c.breedName;
    c.deepCopy(buffer);

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
    static int number;

public:
    //Constructors
    ShelterSpot(int id);
    ShelterSpot(const ShelterSpot& other);
    ShelterSpot();
    //Destructor
    ~ShelterSpot();

    //Getters
    CatCard* getCard() const;
    float getCleanliness() const;
    int getOccupationCount() const;

    //Functions
    bool isEmpty() const;
    void occupy(const CatCard& c);
    void evict();
    void deteriorate();
    void clean();

    //Operator overloads
    ShelterSpot& operator=(const ShelterSpot& other);
    friend ostream& operator<<(ostream& os, const ShelterSpot& s);
    friend istream& operator>>(istream& is, ShelterSpot& s);
};
int ShelterSpot::number = 0;

//Constructors
ShelterSpot::ShelterSpot(int id) : card(nullptr), cleanliness(100), spotID(id){
    logHistory = new int [3]{0, 0, 0};
}
ShelterSpot::ShelterSpot(const ShelterSpot &other) : spotID(other.spotID) {
    card = other.card ? new CatCard(*other.card) : nullptr;
    logHistory = new int [3];
    for (int i = 0; i < 3; i++)
        logHistory[i] = other.logHistory[i];
    cleanliness = other.cleanliness;
}
ShelterSpot::ShelterSpot() : cleanliness(100.0), spotID(0) {
    logHistory = new int [3] {0, 0, 0};
}
//Destructor
ShelterSpot::~ShelterSpot() {
    delete card;
    delete[] logHistory;
}

//Getters
CatCard* ShelterSpot::getCard() const {
    return card;
}
float ShelterSpot::getCleanliness() const {
    return cleanliness;
}
int ShelterSpot::getOccupationCount() const {
    return logHistory[0];
}

//Functions
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

//Operator overloads
ShelterSpot& ShelterSpot:: operator=(const ShelterSpot& other) {
        if(this != &other) {
            delete card;
            card = other.card ? new CatCard(*other.card) : nullptr;
            cleanliness = other.cleanliness;
            for(int i=0; i<3; ++i)
                logHistory[i] = other.logHistory[i];
        }
        return *this;
    }
ostream& operator<<(ostream& os, const ShelterSpot& s) {
    os << "Slot " << s.spotID << ": ";
    if (s.card) os << *s.card; else os << "Empty";
    os << " | Clean: " << s.cleanliness << "% (History: " << s.logHistory[2] << " cleans)";
    return os;
}
istream& operator>>(istream& is, ShelterSpot& s) {
  is >> s.cleanliness;
  return is;
}

/*---------------------------------------------------------------------------------------*/

class Score {
private:
    double balance;
    double multiplier;
    static int totalCatsRescued;
    static double totalSpent;
    const int scoreID;

public:
    //Constructors
    Score(double b, double a);
    Score();
    Score(const Score& other);

    //Getter
    double getBalance() const;

    //Functions
    void addPoints(double amount);
    void recordRescue();
    void recordExpense(double amount);

    //Operator overloads
    Score& operator=(const Score& other);
    friend ostream& operator<<(ostream& os, const Score& e);
    friend istream& operator>>(istream& is, Score& e);
};
int Score::totalCatsRescued = 0;
double Score::totalSpent = 0.0;

//Constructors
Score::Score(double b, double a) : balance(b), multiplier(a), scoreID(totalCatsRescued) {}
Score::Score() : scoreID(1) {
    balance = 0;
    multiplier = 1;
}
Score::Score(const Score &other) : scoreID(totalCatsRescued){
    this->balance = other.balance;
    this->multiplier = other.multiplier;
}

//Getter
double Score::getBalance() const {
    return balance;
}

//Functions
void Score::addPoints(double amount) {
    balance += (amount * multiplier);
}
void Score::recordRescue() { totalCatsRescued++; }
void Score::recordExpense(double amount) { totalSpent += amount;}

//Operator overloads
Score& Score :: operator=(const Score& other) {
    if (this != &other) {
        this->balance = other.balance;
        this->multiplier = other.multiplier;
    }
    return *this;
}
ostream& operator<<(ostream& os, const Score& e) {
    os << " * Purr-Points: " << e.balance<<" * "
    << " | Total Rescued: " << Score::totalCatsRescued
    << " | Total Spent: " << Score::totalSpent<<" pts"<<endl;
    return os;
}
istream& operator>>(istream& is, Score& e) {
    is >> e.balance;
    return is;
}

/*---------------------------------------------------------------------------------------*/

class GameEngine {
private:
    ShelterSpot* spots[6];
    Score stats;
    int difficulty;
    static int maxCatsInOneSpot;
    const int gameID;


public:
    //Constructors
    GameEngine();
    GameEngine(int diff);
    GameEngine(const GameEngine& o);
    //Destructor
    ~GameEngine();

    //Getters
    const ShelterSpot& getSpot(int index) const;
    const Score& getScore() const;

    //Functions
    bool canMergeAny() const;
    bool isShelterFull() const;
    int spawnCat();
    int attemptMerge(int a, int b);
    int cleanSlot(int id);
    int getDifficultyByCatSpot() const {
        if (maxCatsInOneSpot < 3) return 1;
        if (maxCatsInOneSpot < 6) return 2;
        if (maxCatsInOneSpot < 10) return 3;
        return 5;
    }

    //Operator overloads
    GameEngine& operator=(const GameEngine& other);
    friend ostream& operator<<(ostream& os, const GameEngine& ge);
    friend istream& operator>>(istream& is, GameEngine& ge);
};
int GameEngine::maxCatsInOneSpot = 0;

//Constuctors
GameEngine::GameEngine() : stats(10.0, 1.0), difficulty(0), gameID(1) {
    for (int i = 0; i < 6; ++i) spots[i] = new ShelterSpot(i);
}
GameEngine::GameEngine(int diff) : stats(0.0, 1.5), difficulty(diff), gameID(1) {
    for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(i);
}
GameEngine::GameEngine(const GameEngine& o) : stats(o.stats), difficulty(o.difficulty), gameID(1) {
    for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(*o.spots[i]);
}
//Destructor
GameEngine::~GameEngine() {
    for (int i = 0; i < 6; ++i) delete spots[i];
}

//Getters
const ShelterSpot& GameEngine::getSpot(int index) const { return *spots[index]; }
const Score& GameEngine::getScore() const { return stats; }

//Functions
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
int GameEngine::spawnCat() {
    for (int i = 0; i < 6; ++i) {
        if (spots[i]->isEmpty()) {
            if (spots[i]->getCleanliness() > 20.0f) {
                spots[i]->occupy(CatCard(1, false, 'F'));
                int currentSpotTotal = spots[i]->getOccupationCount();
                    if (currentSpotTotal > maxCatsInOneSpot) {
                        maxCatsInOneSpot = currentSpotTotal;
                    }
                stats.recordRescue();
                for (int j=0; j<6; j++){
                    spots[j]->deteriorate();
                }
                return 0;
            }
            return 1;
        }
    }
    return 2;
}
int GameEngine::attemptMerge(int a, int b) {
    if (a >= 0 && a < 6 && b >= 0 && b < 6 && a != b && !spots[a]->isEmpty() && !spots[b]->isEmpty()) {
        if (spots[a]->getCard()->getTier() == spots[b]->getCard()->getTier()) {
            int nextTier = spots[a]->getCard()->getTier() + 1;
            spots[a]->occupy(CatCard(nextTier, false, 'M'));
            spots[b]->evict();
            stats.addPoints(10.0);
            for(int j=0; j<6; j++) spots[j]->deteriorate();
            return 1;
        }
    }
    return 0;
}
int GameEngine::cleanSlot(int const id) {
    if (stats.getBalance() >= 5.0) {
        stats.addPoints(-5.0);
        stats.recordExpense(5.0);
        spots[id]->clean();
        return 1;
    }
        return 0;
}

//Operator overloads
GameEngine& GameEngine::operator=(const GameEngine& other) {
    if(this != &other) {
        for(int i=0; i<6; ++i) delete spots[i];
        stats = other.stats; difficulty = other.difficulty;
        for(int i=0; i<6; ++i) spots[i] = new ShelterSpot(*other.spots[i]);
    }
    return *this;
}
ostream& operator<<(ostream& os, const GameEngine& ge) {
    return os << ge.difficulty;
}
istream& operator>>(istream& is, GameEngine& ge) {
    return is >> ge.difficulty;
}

/*---------------------------------------------------------------------------------------*/

class Menu {
private:
    GameEngine game;
    bool isRunning;
    static int max_tier_spot;

public:
    Menu();

    void play() {
        system("CLS");
        cout << "=============================================================" << endl;
        cout << "                     CAT SHELTER MANAGER                     " << endl;
        cout << "=============================================================" << endl;
        int valueOption;
        while (isRunning) {
            cout<<"\n";
            cout<<"                              / __/                             "<<endl;
            cout<<"<--------------------------- (     ) --------------------------->"<<endl;
            cout<<"                               >+<                               "<<endl;

            cout << "\n" << game.getScore() << "\n";
            for (int i=0; i<6; ++i)
                cout<< game.getSpot(i) << endl;

            if (game.isShelterFull() && !game.canMergeAny()) {
                cout<<"\033[2J\033[H"<<"\n!!! GAME OVER !!! No more moves possible.\n \n Your stats:\n "<<game.getScore()<<endl;
                isRunning = false;
                break;
            }

            cout << "1:Rescue | 2:Merge | 3:Clean | 4:Exit\nChoice: ";

            int choice; cin>>choice;
            if (cin.fail()) {
                cin.clear();
                choice = 0;
            }
            cin.ignore(1000, '\n');

            switch (choice) {
                case 1:
                {valueOption = game.spawnCat();
                    if (valueOption == 0){
                        cout<<"\033[2J\033[H"<<"\n Success: Cat has been rescued!\n";
                    }
                    else
                        if (valueOption == 1) {
                            cout<<"\033[2J\033[H"<<"\n Oh-no: The spot is too dirty!\n";
                        }
                        else
                            cout<<"\033[2J\033[H"<<"\n No more room in the shelter!\n";
                }
                break;

                case 2:
                { int a, b;
                    cout << "Enter IDs to merge: ";
                    cin >> a >> b;
                    if (!cin.fail()) {
                        valueOption = game.attemptMerge(a, b);
                        if (valueOption) {
                            cout<<"\033[2J\033[H"<<"\n The cats have been merged!\n";
                            for (int i=0; i<6; ++i)
                                if (!game.getSpot(i).isEmpty() and game.getSpot(i).getCard()->getTier() == 6 and i != max_tier_spot) {
                                    cout<<"\n *** CONGRATULATIONS! You got the Sphinx!***"<<endl;
                                    max_tier_spot = i;
                                }
                        }
                        else
                            cout<<"\033[2J\033[H"<<"\n Merge Invalid!\n";
                    }
                    else {
                        cin.clear();
                        cout<<"\033[2J\033[H"<<"\n Merge Invalid!\n";
                    }
                    cin.ignore(1000, '\n');
                }
                break;

                case 3:
                {int id;
                    cout << "Enter the spot to clean: ";
                    cin >> id;
                    if (!cin.fail()) {
                        valueOption = game.cleanSlot(id);
                        if (valueOption) {
                            cout<<"\033[2J\033[H"<<"\n The spot has been cleaned!\n";
                        }
                        else
                            cout<<"\033[2J\033[H"<<"\n Oh-no: Not enough points!";
                        if (valueOption == 0 and !game.canMergeAny()) {
                            cout<<"\033[2J\033[H"<<"\n!!! GAME OVER !!! No more moves possible.\n \n Your stats:\n "<<game.getScore();
                            isRunning = false;
                            break;
                        }
                    }
                    else {
                        cin.clear();
                        cout<<"\033[2J\033[H"<<"\n Cleaning Invalid!\n";
                    }
                    cin.ignore(1000, '\n');
                }
                break;

                case 4: isRunning = false; cout<<"\033[2J\033[H"<<"\n Your stats:\n"<<game.getScore();
                    break;
                default:  cout<<"\033[2J\033[H"<<"\n It is an invalid input, please use a key supported.\n";
            }
        }

    }
};
int Menu::max_tier_spot = -1;

Menu::Menu() : isRunning(true){ }

int main() {
    Menu playMenu;
    playMenu.play();

    return 0;
}