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

};

/*---------------------------------------------------------------------------------------*/

class Economy {
    //handling the point system
};

/*---------------------------------------------------------------------------------------*/

class GameEngine {
    //running the game
};


int main() {
    CatCard testCat(1, false, 'F', "Tabby");
    testCat.setTier(2);
    cout << "Testing Getter: " << testCat.getBreed() << " Tier: " << testCat.getTier() << "\n";
    cout<<testCat<<endl;

    CatCard testCat2;
    cin>>testCat2;
    cout << testCat2 << "\n";

    return 0;
}