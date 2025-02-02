#include   <iostream >
#include <string>
#include <mutex>
#include <regex>
#include <vector>
#include <thread>
using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* ispisRazreda[]{ "PRVI"," DRUGI","TRECI","CETVRTI" };

bool  ValidirajEmail(string email){
    return regex_search(email, regex("^[a-z]+(\\.?[a-z]+)?@(edu.fit|fit)\\.(ba|com|org)$"));
}

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;

   
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;

    }
    Kolekcija(const Kolekcija& obj) :_trenutno(obj._trenutno),_omoguciDupliranje(obj._omoguciDupliranje){
        if (_trenutno > 0) {
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];


            for (int i = 0; i < _trenutno; i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        else {
            _elementi1 = nullptr;
            _elementi2 = nullptr;
        }
    }
    Kolekcija& operator= (const Kolekcija& obj)
    {
        if (this != &obj) {
            if (_trenutno != 0) {
                delete[] _elementi1;
                delete[] _elementi2;
            }
            _trenutno = obj._trenutno;
            _omoguciDupliranje = obj._omoguciDupliranje;
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];
            for (int i = 0; i < _trenutno; i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }

        }return *this;
    }
    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() { return _trenutno; }
    void AddElement(T1 el1, T2 el2) {
        
        for (int i = 0; i < _trenutno; i++)
        {
            
           
            if (!_omoguciDupliranje && ( _elementi1[i] == el1 && _elementi2[i] == el2))
                throw exception(" ne mogu se dodavati isti elementi!!");
        }
               
        
        T1* temp1 = new T1[_trenutno + 1];
        T2* temp2 = new T2[_trenutno + 1];
        for (int i = 0; i < _trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        delete[] _elementi1;
        delete[] _elementi2;
        temp1[_trenutno] = el1;
        temp2[_trenutno] = el2;
        _elementi1 = temp1;
        _elementi2 = temp2;
        _trenutno++;
        temp1 = nullptr;
        temp2 = nullptr;
    }
    Kolekcija<int, int>operator() (int pocetak, int kraj) {
        Kolekcija<int, int> nova;
        if (pocetak <= 0 && kraj > _trenutno)
            throw exception(" lokacija nije validna!");
        for (int i = pocetak; i <= kraj; i++)
        {
            nova.AddElement(_elementi1[i], _elementi2[i]);
        }return nova;
    }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj) {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum& operator= (const Datum& obj) {
        if (this != &obj) {
            *_dan = *obj._dan;
            *_mjesec = *obj._mjesec;
            *_godina = *obj._godina;
        }return *this;
    }
    int toDays() {
        return ((*_dan) + (*_mjesec * 30 )+( *_godina * 365));
    }
    bool operator ==(const Datum& obj) {
        return (*obj._dan == *_dan && *obj._mjesec == *_mjesec && *obj._godina == *_godina);
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
};

class Predmet {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<int, Datum> _ocjene;
public:
    Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
        _naziv = GetNizKaraktera(naziv);
        if (ocjena > 0)
            _ocjene.AddElement(ocjena, datum);
    }
    Predmet(const Predmet& obj):_naziv(GetNizKaraktera(obj._naziv)),_ocjene(obj._ocjene) {
        
    }
    Predmet& operator=(const Predmet& obj) {
        if (this != &obj) {
            delete[] _naziv;
            _naziv = GetNizKaraktera(obj._naziv);
            _ocjene = obj._ocjene;
        }return *this;
    }
    float ProsjekUPredmetu() {
        float prosjek = 0.0f;
        for (int i = 0; i <_ocjene.getTrenutno(); i++)
        {
            prosjek += _ocjene.getElement1(i);
        }return prosjek / _ocjene.getTrenutno();
    }
    bool operator==(const Predmet& obj) {
        for (int i = 0; i < _ocjene.getTrenutno(); i++)
        {

            if (_ocjene.getElement1(i) == obj._ocjene.getElement1(i) && _ocjene.getElement2(i) == obj._ocjene.getElement2(i))
                return true;

        }
        if (strcmp(obj._naziv, obj._naziv) == 0)
            return true;
        return false;

    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    void AddOcjena(int ocjena, Datum datum) {
        _ocjene.AddElement(ocjena, datum);
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }friend ostream& operator<< (ostream& COUT,  Predmet& obj) {
        COUT << " naziv :" << obj._naziv << endl;
        for (int i = 0; i < obj._ocjene.getTrenutno(); i++)
        {
            COUT << " ocjena i datum\n " << " (" << obj._ocjene.getElement1(i) << ") " << obj._ocjene.getElement2(i) << endl;
        } COUT << " prosjek :" << obj.ProsjekUPredmetu() << endl;
        return COUT;

    }
};
class Uspjeh {
    eRazred _razred;
    //string se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>();
    }
    Uspjeh(const Uspjeh& obj) :_razred(obj._razred){

        _polozeniPredmeti = new Kolekcija<Predmet, string>(*obj._polozeniPredmeti);
    }
    Uspjeh& operator=( const Uspjeh& obj){
        if (this != &obj) {
            _razred = obj._razred;
            *_polozeniPredmeti = *obj._polozeniPredmeti;
        }
            return *this;
    }
    ~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
    float ProsjekUUspjehu() {
        float prosjek = 0.0f;
        for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++)
        {
            prosjek += _polozeniPredmeti->getElement1(i).ProsjekUPredmetu();
        } return prosjek / _polozeniPredmeti->getTrenutno();
    }
    Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << ispisRazreda[obj._razred] << " " << *obj._polozeniPredmeti << endl;
        return COUT;
    }
};
mutex mu;

class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void SaljiMail(eRazred razred, float prosjekRazreda, float prosjekSkolovanja) {
        mu.lock();
        cout << "FROM:info@fit.ba\n TO :" << GetEmail() << " Postovani " << GetImePrezime() << ", evidentirali ste uspjeh za" << ispisRazreda[razred] << ".Dosadasnji uspjeh(prosjek)  na nivou" << ispisRazreda[razred] << " razreda iznosi " << prosjekRazreda << ", a ukupni uspjeh u toku skolovanja iznosi" << prosjekSkolovanja << ".\n Pozdrav.\nFIT Team." << endl;
        mu.unlock();
    }
    void SaljiSMS(eRazred razred, float prosjekRazreda) {
        mu.lock();
        cout << "Svaka cast za uspjeh " << prosjekRazreda << " u" << ispisRazreda[razred] << " razredu." << endl;
        mu.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        _brojTelefona = brojTelefona;
        if (ValidirajEmail(emailAdresa) == true)
            _emailAdresa = emailAdresa;
        else _emailAdresa = "notSet@edu.fit.ba";
    }
    Kandidat(const Kandidat& obj) :_imePrezime(GetNizKaraktera(obj._imePrezime)), _brojTelefona(obj._brojTelefona), _emailAdresa(obj._emailAdresa), _uspjeh(obj._uspjeh) {}
    Kandidat& operator=(const Kandidat& obj) {
        if (this != &obj) {
            delete[] _imePrezime;
            _imePrezime = GetNizKaraktera(obj._imePrezime);
            _brojTelefona = obj._brojTelefona;
            _emailAdresa = obj._emailAdresa;
            _uspjeh = obj._uspjeh;
        }return *this;
    }
    ~Kandidat() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    float  ukupniProsjek(eRazred razred) {
        float prosjek = 0.0f;
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            if (_uspjeh[i].GetERazred() == razred)
                for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
                {
                    prosjek += _uspjeh[i].GetPredmeti()->getElement1(j).ProsjekUPredmetu();

                }
            return prosjek / _uspjeh[i].GetPredmeti()->getTrenutno();
        }
    }
    bool jesul(eRazred razred, Predmet& predmet) {
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            if(razred== _uspjeh[i].GetERazred())
                for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
                {
                    if (predmet == _uspjeh[i].GetPredmeti()->getElement1(j))
                        return true;
                }
        }
        return false;
    }
    bool AddPredmet(eRazred razred, Predmet& predmet, string napomena) {
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            if (_uspjeh[i].GetERazred() == razred) {
               
                if (jesul(razred, predmet))
                        return false;
                

                if (predmet.ProsjekUPredmetu() < 2.5)
                    return false;

                if (_uspjeh[i].GetPredmeti()->getTrenutno() >= 5)
                        return false;
            }

            _uspjeh[i].GetPredmeti()->AddElement(predmet, napomena);

            float ukupnni = predmet.ProsjekUPredmetu();
            float prosjek = ukupniProsjek(razred);

            thread t1(&Kandidat::SaljiMail, this, razred, prosjek, ukupnni);
            t1.join();

            if (ukupniProsjek(razred) >= 4.5) {
                thread t2(&Kandidat::SaljiSMS, this, razred, prosjek);
                t2.join();
            }

        }
        Uspjeh u(razred);
        u.GetPredmeti()->AddElement(predmet, napomena);
        _uspjeh.push_back(u);

        float ukupnni = predmet.ProsjekUPredmetu();
        float prosjek = ukupniProsjek(razred);

        thread t3(&Kandidat::SaljiMail, this, razred, prosjek, ukupnni);
        t3.join();

        return true;

    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    float ProsjecanBRDana() {
        float prosjek = 0.0f;
         float prosjek2 = 0.0f;
         Predmet predmet;

        for (int i = 0; i < _uspjeh.size(); i++)
        {
            for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
            {
                 predmet = _uspjeh[i].GetPredmeti()->getElement1(j);
                for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(i).GetOcjene().getTrenutno(); k++)
                {
                    prosjek += _uspjeh[i].GetPredmeti()->getElement1(i).GetOcjene().getElement2(k).toDays();
                }
                cout << " njegov prosjek dana" << prosjek / _uspjeh[i].GetPredmeti()->getTrenutno() << endl;
                prosjek2 += predmet.GetOcjene().getElement2(i).toDays();
                cout << " moj prosjek dana" << prosjek2 / _uspjeh[i].GetPredmeti()->getTrenutno() << endl;
                return prosjek / _uspjeh[i].GetPredmeti()->getTrenutno();
            }

        }
    }
    Kolekcija<Predmet, float> operator()(Datum d1, Datum d2) {
        //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
   //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
        Kolekcija<Predmet, float> nova;
        float prosjek = ProsjecanBRDana();
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
            {
                for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(i).GetOcjene().getTrenutno(); k++) {

                    if (_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k).toDays() > d1.toDays() && _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k).toDays() < d2.toDays())
                        nova.AddElement(_uspjeh[i].GetPredmeti()->getElement1(j), prosjek);

                }
            }
        }
        return nova;
    }
    Uspjeh* operator [](eRazred razred)
    {
        Uspjeh* u = nullptr;
        for (int i = 0; i < _uspjeh.size(); i++)
        {
            if (_uspjeh[i].GetERazred() == razred)
                u = &_uspjeh[i];
        } return u;
       /* Uspjeh* uspjeh_Irazred = jasmin[PRVI];*///vraca uspjeh kandidata ostvaren u prvom razredu}
    }
};


void main() {

  
    Datum temp,
        datum19062019(19, 6, 2019),
        datum20062019(20, 6, 2019),
        datum30062019(30, 6, 2019),
        datum05072019(5, 7, 2019);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
    ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
    Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
    cout << kolekcija2 << crt;
    try {
        //primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
        Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
        Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

    //parametri: nazivPredmeta, prva ocjena, datum
    Predmet Matematika("Matematika", 5, datum19062019),
        Fizika("Fizika", 5, datum20062019),
        Hemija("Hemija", 2, datum30062019),
        Engleski("Engleski", 5, datum05072019);

    Matematika.AddOcjena(3, datum05072019);
    Matematika.AddOcjena(5, datum05072019);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
    cout << Matematika << endl;

    if (ValidirajEmail("text.text@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.org"))
        cout << "Email validan" << crt;

    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */

    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
    Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
    cout << jasminUspjeh << crt;

    Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
    if (uspjeh_Irazred != nullptr)
        cout << *uspjeh_Irazred << crt;

    cin.get();

    system("pause>0");
}