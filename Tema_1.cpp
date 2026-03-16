#include <iostream>
#include <cstring>
using namespace std;


class CatCard {
private:
    int tier;
    bool isRare;
    char gender; //F or M
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
    os << (c.breedName ? c.breedName : "Unknown") << " [T" << c.tier << "] (" << c.gender << ")";
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
    const int slotID;

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
    friend std::ostream& operator<<(std::ostream& os, const ShelterSpot& s);
};

ShelterSpot::ShelterSpot(int id) : card(nullptr), slotID(id) {
    logHistory = new int[3]{0, 0, 0};
}
ShelterSpot::ShelterSpot(const ShelterSpot &other) : slotID(other.slotID) {
    card = other.card ? new CatCard(*other.card) : nullptr;
    logHistory = new int[3];
    for (int i = 0; i < 3; i++)
        logHistory[i] = other.logHistory[i];
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
std::ostream& operator<<(std::ostream& os, const ShelterSpot& s) {
    os << "Slot " << s.slotID << ": ";
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
    friend std::ostream& operator<<(std::ostream& os, const Economy& e);
};

Economy::Economy(double b, double m) : balance(b), multiplier(m) {}

double Economy::getBalance() const { return balance; }
void Economy::addFunds(double amount) { balance += (amount * multiplier); }

std::ostream& operator<<(std::ostream& os, const Economy& e) {
    os << "Purr-Points: " << e.balance;
    return os;
}

/*---------------------------------------------------------------------------------------*/

class GameEngine {
    //running the game
};


int main() {


    return 0;
}