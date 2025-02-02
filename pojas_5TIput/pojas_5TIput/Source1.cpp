#include <iostream>
#include <mutex>
#include <regex>
#include <thread>
#include <string>
#include <vector>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const char* IspisPojasa[]{ "BIJELI", "ZUTI","NARANDZASTI","ZELENI","PLAVI","SMEDJI","CRNI" };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

bool ValidirajLozinku(string lozinka) {
	/*najmanje 7 znakova
		- velika i mala slova
		- najmanje jedan broj
		- najmanje jedan specijalni znak*/
	return regex_search(lozinka, regex(" (?=.{7,})(?=.*[A-Z]{1,})(?=.*[a-z]{1,})(?=.*[\\d]{1,})(?=.*[._-]{1,})"));
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

template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() { _trenutno = new int(0); }
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(*obj._trenutno);
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			for (int i = 0; i < *_trenutno; i++)
			{
				delete[] _elementi1[i]; _elementi1[i] = nullptr;
				delete[] _elementi2[i]; _elementi2[i] = nullptr; 
			}
			delete _trenutno; _trenutno = nullptr;
			_trenutno = new int(*obj._trenutno);
			for (int i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}return *this;
	}
	void AddElement(T1 el1, T2 el2) {
		if (*_trenutno >= max)
			throw exception(" niz je popunjen");
		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[*_trenutno] = new T2(el2);
		(*_trenutno)++;
	}

	~Kolekcija() {

		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	//na osnovu vrijednosti T1 mijenja vrijednost T2. 
	T2& operator[] (T1 el1) {
		for (int i = 0; i < *_trenutno; i++)
		{
			if (*_elementi1[i] == el1)
				return *_elementi2[i];
		}
	}
	void  AddElement(T1 el1, T2 el2, int lokacija) {
		for (int i = lokacija; i < *_trenutno; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];

		}
		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;
	}
	void RemoveAt(int lokacija) {
		for (int i = *_trenutno; i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		(*_trenutno)--;
	}
	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = &dan;
		_mjesec = &mjesec;
		_godina = &godina;
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);

	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {

			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);

		}return *this;
	}
	int toDays() {
		return (*_dan + *_mjesec * 30 + *_godina * 365);
	}
	bool operator== (const Datum& obj) {
		return (*_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << obj._dan << "." << obj._mjesec << "." << obj._godina;
		return COUT;
	}
};

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
	Kolekcija<int, Datum*, brojTehnika>* _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
		_ocjene = new Kolekcija<int, Datum*, brojTehnika>();

	}
	Tehnika(const Tehnika& obj) {
		_naziv = GetNizKaraktera( obj._naziv);
		_ocjene = new Kolekcija<int, Datum*, brojTehnika>(*obj._ocjene);
	}
	bool operator==(const Tehnika& obj) {
		for (int i = 0; i < _ocjene->getTrenutno(); i++)
		{
			if (_ocjene->getElement1(i) == obj._ocjene->getElement1(i))
				if (_ocjene->getElement2(i) == obj._ocjene->getElement2(i))
					return true;
		}
		if (strcmp(_naziv, obj._naziv) == 0)
			return true;
		return false;
	}
	Tehnika& operator=(const Tehnika& obj) {
		if (this != &obj) {
			delete[] _naziv;
			delete _ocjene;
			_naziv = GetNizKaraktera(obj._naziv);
			_ocjene = new Kolekcija<int, Datum*, brojTehnika>(*obj._ocjene);
		}return *this;
	}
	float ProsjekUTehnici() {
		float prosjek = 0.0f;
		for (int i = 0; i < _ocjene->getTrenutno(); i++)
		{
			prosjek += _ocjene->getElement1(i);
		} return prosjek / _ocjene->getTrenutno();
	}
	bool AddOcjena(int ocjena, Datum datum) {
		/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
		-   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
		-   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
	*/
		if (ocjena < 1 && ocjena>5)
			throw exception(" ocjena nije u odogvarajucem rangu");
		for (int i = 0; i < _ocjene->getTrenutno(); i++)
		{
			if ( abs(datum.toDays()-_ocjene->getElement2(i)->toDays() )> 3)
				if (_ocjene->getElement2(i)->toDays() < datum.toDays()) {
					return true;
					_ocjene->AddElement(ocjena, new Datum(datum));
				}
		}return false;
	}

	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjene; _ocjene = nullptr;
	}

	
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return *_ocjene; }
	friend ostream& operator << (ostream& COUT,  Tehnika& obj) {
		COUT << " Naziv:" << obj.GetNaziv()<< endl;
		COUT << " ocjene i datumi " << endl;
		for (size_t i = 0; i < obj.GetOcjene().getTrenutno(); i++)
		{
			COUT << "(" << obj.GetOcjene().getElement1(i)<< ")  -  " << *obj.GetOcjene().getElement2(i) << endl;
		}
		COUT << "prosjecna ocjena" << obj.ProsjekUTehnici() << endl;
		return COUT;
	 }
};

class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	Polaganje(const Polaganje& obj) {
		_pojas = obj._pojas;
		_polozeneTehnike = obj._polozeneTehnike;

	}
	Polaganje& operator= (const Polaganje& obj) {
		if (this != &obj) {
			for (int i = 0; i < _polozeneTehnike.size(); i++)
			{
				delete[] _polozeneTehnike[i];
				_polozeneTehnike[i] = nullptr;
			}
			_pojas = obj._pojas;
			for (int i = 0; i < _polozeneTehnike.size(); i++)
			{
				_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
			}
		}return *this;
	}
	float ProsjekUPolaganju() {
		float prosjek = 0.0f;
		for (int i = 0; i < _polozeneTehnike.size(); i++)
		{
			prosjek += _polozeneTehnike[i]->ProsjekUTehnici();

		}
		 return prosjek / _polozeneTehnike.size();
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << IspisPojasa[obj._pojas ]<< endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}
};

class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
	}
	Korisnik(const Korisnik& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
	}
	Korisnik& operator=(const Korisnik& obj) {
		if (this != &obj) {
			delete[] _imePrezime;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_emailAdresa = obj._emailAdresa;
			_lozinka = obj._lozinka;
		}return *this;
	}
	 virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
};
mutex mu;
class KaratePolaznik :public Korisnik {
	vector<Polaganje> _polozeniPojasevi;

	void SaljiMail( Pojas pojas, const char* tehnika,  float prosjekNaPojasu, float ukpniProsjek) {
		mu.lock();
		cout << "FROM:info@karate.ba\nTO: emailKorisnika\nPostovani " << GetImePrezime() << ", evidentirana vam je thenika " << tehnika << " za " << IspisPojasa[pojas] << "pojas.Dosadasnji uspjeh(prosjek ocjena) na pojasu" << IspisPojasa[pojas] << " iznosi " << prosjekNaPojasu << ", a ukupni uspjeh(prosjek ocjena) na svim pojasevima iznosi " << ukpniProsjek << ".\nPozdrav.\nKARATE Team." << endl;
		cout << endl;
		mu.unlock();
	}
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka):Korisnik(imePrezime,emailAdresa,lozinka) {
		
	}
	virtual void Info() {
		cout << *this;
	}
	bool Isti(Pojas pojas, Tehnika tehnika) {
		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
		if(pojas== _polozeniPojasevi[i].GetPojas())
			for (int i = 0; i < _polozeniPojasevi[i].GetTehnike().size(); i++)
			{
				if (tehnika.GetNaziv() == _polozeniPojasevi[i].GetTehnike()[i]->GetNaziv())
					return true;

			}

		}
		return false;

	}
	float ukupniProsjek() {
		float prosjke = 0.0f;
		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			prosjke += _polozeniPojasevi[i].ProsjekUPolaganju();
		}return prosjke / _polozeniPojasevi.size();
	}
	bool AddTehniku(Pojas pojas, Tehnika tehnika) {

		/*sve tehnike na nivou jednog pojasa(ZUTI, ZELENI ...) se evidentiraju unutar istog objekta tipa Polaganje,
			tom prilikom onemoguciti :
		-dodavanje istih(moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
			-dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
			(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)*/
		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{

			if (pojas == _polozeniPojasevi[i].GetPojas()) {
				

				if (pojas > _polozeniPojasevi[i].GetPojas() && _polozeniPojasevi[i].GetTehnike().size() < 3 || _polozeniPojasevi[i].ProsjekUPolaganju() < 3.5)
					return false;
				_polozeniPojasevi[i].GetTehnike().push_back(new Tehnika(tehnika));
				float prvi = _polozeniPojasevi[i].ProsjekUPolaganju();
				float drugi = ukupniProsjek();
				const char* tehnika1 = tehnika.GetNaziv();
				thread t1(&KaratePolaznik::SaljiMail, this, pojas, tehnika1, prvi, drugi);
				t1.join();
		     }
		}
		
			Polaganje p(pojas);
			p.GetTehnike().push_back(new Tehnika(tehnika));
			float prvi = p.ProsjekUPolaganju();
			float drugi = ukupniProsjek();
			const char* tehnika1 = tehnika.GetNaziv();
			thread t2(&KaratePolaznik::SaljiMail, this, pojas, tehnika1, prvi, drugi);
			t2.join();
			return true;
	}
	KaratePolaznik(const KaratePolaznik& obj) :Korisnik(obj){
		_polozeniPojasevi = obj._polozeniPojasevi;
		
	 }
	~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
};



void main() {


	Datum   datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);

	int kolekcijaTestSize = 10;

	Kolekcija<int, int> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);

	cout << kolekcija1 << endl;

	try {
		/*metoda AddElement baca izuzetak u slucaju da se pokusa
		dodati vise od maksimalnog broja elemenata*/
		kolekcija1.AddElement(11, 11);
	}
	catch (exception& err) {
		cout << crt << "Greska -> " << err.what() << crt;
	}
	cout << kolekcija1 << crt;


	kolekcija1.RemoveAt(2);
	/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa proslijedjenim indeksom.
	nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar kolekcije tj.
	pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za jedno mjesto unazad
	npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti sljedeci
	0 0
	2 2
	3 3
	*/

	cout << kolekcija1 << crt;

	kolekcija1.AddElement(9, 9, 2);
	/*funkciji AddElement se, kao treci parametar, moze proslijediti i lokacija na koju se dodaju
	nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene za jedno mjesto unaprijed
	u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce biti sljedeci
	0 0
	9 9
	1 1
	2 2
	3 3
	*/

	cout << kolekcija1 << crt;

	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija1 << crt;

	//na osnovu vrijednosti T1 mijenja vrijednost T2. 
	kolekcija1[9] = 2;
	/* npr.ako unutar kolekcije postoje elementi:
	0 0
	9 9
	1 1
	2 2
	3 3
	nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
	0 0
	9 2
	1 1
	2 2
	3 3
	*/

	Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");

	/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
		-   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
		-   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
	*/
	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;

	/* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
	   ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
	cout << choku_zuki << endl;

	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;

	/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	-   najmanje 7 znakova
	-   velika i mala slova
	-   najmanje jedan broj
	-   najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/

	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
	Korisnik* emailNijeValidan = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");

	/*
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/

	//doraditi klase da nacin da omoguce izvrsenje naredne linije koda
	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;

		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;
	}

	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
	na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
	Pozdrav.
	KARATE Team.
	slanje email poruka implemenitrati koristeci zasebne thread-ove.
	*/

	//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
	delete jasmin;
	delete adel;
	delete emailNijeValidan;

	cin.get();
	system("pause>0");
}