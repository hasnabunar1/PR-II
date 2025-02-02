#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>
using namespace std;


const char* crt = "\n-------------------------------------------\n";
enum Karakteristike { NARUDZBA, KVALITET, PAKOVANJE, ISPORUKA };
const char* IspisKarakteristike[]{ "NARUDZBA"," KVALITET","PAKOVANJE"," ISPORUKA" };

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
class Rjecnik {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Rjecnik(bool omoguciDupliranje = true) {
		_omoguciDupliranje = omoguciDupliranje;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Rjecnik& operator=(const Rjecnik& obj) {
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
	Rjecnik(const Rjecnik& obj) : _trenutno(obj._trenutno),_omoguciDupliranje(obj._omoguciDupliranje){
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
	void AddElement(T1 el1, T2 el2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[]  _elementi1;
		delete[]  _elementi2;
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = nullptr;
		temp2 = nullptr;
		_trenutno++;
	} 
	
	~Rjecnik() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }
	friend ostream& operator<< (ostream& COUT, const Rjecnik& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	Rjecnik<int, int> operator()(int pocetak, int kraj) {
		/*
		* metoda prihvata indekse lokacije, te vraća elemente koji se u rjecniku nalaze na proslijedjenim lokacijama (ukljucujuci i te lokacije)
		*/Rjecnik<int, int> novi;
		/*Rjecnik<int, int> rjecnik2 = rjecnik1(2, 5);*/
		for (int i = pocetak; i <=kraj; i++)
		{
			novi.AddElement(_elementi1[i], _elementi2[i]);
		}
		return novi;
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
	bool operator==(const Datum& obj) {
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

class ZadovoljstvoKupca {
	int _ocjena; //za svaku kupovinu kupac moze ostaviti jednu ocjenu
	//uz ocjenu, kupci mogu komentarisati svaku od karakteristika proizvoda.
	//onemoguciti dupliranje karakteristika tj. svaka karakteristika se moze komentarisati samo jednom...u suprotnom baciti objekat tipa exception
	Rjecnik<Karakteristike, const char*> _komentariKarakteristika;
public:
	ZadovoljstvoKupca(int ocjena = 0) {
		_ocjena = ocjena;
	}
	ZadovoljstvoKupca(const ZadovoljstvoKupca& obj) :_ocjena(obj._ocjena),_komentariKarakteristika(obj._komentariKarakteristika){

	}
	ZadovoljstvoKupca& operator=(const ZadovoljstvoKupca& obj) {
		if (this != &obj) {
			_ocjena = obj._ocjena;
			_komentariKarakteristika = obj._komentariKarakteristika;
		}return *this;
	}
	int GetOcjena() { return _ocjena; }
	Rjecnik<Karakteristike, const char*>& GetKomentareKarakteristika() { return _komentariKarakteristika; }
	friend ostream& operator<<(ostream& COUT, ZadovoljstvoKupca& obj) {
		COUT << "ocjena: (" << obj._ocjena << ") " << endl;
		for (int i = 0; i < obj._komentariKarakteristika.getTrenutno(); i++)
		{
			COUT << " karakteristika: " << obj._komentariKarakteristika.getElement1(i) << " - komentar: " << obj._komentariKarakteristika.getElement2(i) << endl;
		}return COUT;
	} 

	void  DodajKomentarKarakteristike(Karakteristike kar, const char* text) {
		//zadovoljstvoKupca.DodajKomentarKarakteristike(PAKOVANJE, "Pakovanje je bio osteceno");
		for (int i = 0; i < _komentariKarakteristika.getTrenutno(); i++)
		{
			if (kar == _komentariKarakteristika.getElement1(i))
				throw exception("Karakteristika je vec komentarisana!");
		}
			_komentariKarakteristika.AddElement(kar, text);
	}
};

class Osoba {
protected:
	char* _imePrezime;
	Datum _datumRodjenja;
public:
	Osoba(const char* imePrezime = "", Datum datumRodjenja = Datum()) : _datumRodjenja(datumRodjenja) {
		_imePrezime = GetNizKaraktera(imePrezime);
	}
	Osoba(const Osoba& obj): _imePrezime(GetNizKaraktera(obj._imePrezime)),_datumRodjenja(obj._datumRodjenja){}
	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_datumRodjenja = obj._datumRodjenja;

		}return* this;
	}
	  ~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	char* GetImePrezime() { return _imePrezime; }
	Datum& GetDatumRodjenja() { return _datumRodjenja; }

	virtual void Info() = 0;
	friend ostream& operator<< (ostream& COUT, const Osoba& obj) {
		COUT << obj._imePrezime << " " << obj._datumRodjenja << endl;
		return COUT;
	}
};
mutex mu;
class Kupac: public Osoba {
	char* _emailAdresa;
	//float se odnosi na iznos racuna za odredjenu kupovinu
	Rjecnik<float, ZadovoljstvoKupca>* _kupovine;
	vector<int> _bodovi; // bodovi sakupljeni tokom kupovine, svakih potrosenih 10KM donosi po 1 bod.
	void SaljiMail(int ostvareni, int ukupni) {
		mu.lock();
		cout << "\nTo: denis@fit.ba\n\nSubject: Osvareni bodovi\n\nPostovani,\n Prilikom posljednje kupovine ste ostvarili  " << ostvareni << " bodova, tako da trenutno vas ukupan broj bodova iznosi  " << ukupni << ".\n\nZahvaljujemo vam na kupovini.\nPuno pozdrava" << endl;
		mu.unlock();
	}
public:
	Kupac(const char* imePrezime = "", Datum datumRodjenja = Datum(), const char* emailAdresa = "") {
		_emailAdresa = GetNizKaraktera(emailAdresa);
		_kupovine = new Rjecnik<float, ZadovoljstvoKupca>(false);
	}
	~Kupac()
	{
		delete[] _emailAdresa; _emailAdresa = nullptr;
		delete _kupovine; _kupovine = nullptr;
	}
	void Info() {
		cout << *this;
	}
	char* GetEmail() { return _emailAdresa; }
	Rjecnik<float, ZadovoljstvoKupca>& GetKupovine() { return *_kupovine; }
	vector<int> GetBodovi() { return _bodovi; }
	int GetBodoviUkupno() {
		int ukupno = 0;
		for (size_t i = 0; i < _bodovi.size(); i++) ukupno += _bodovi[i];
		return ukupno;
	}
	void DodajKupovinu(int iznosBodova, ZadovoljstvoKupca kritika) {
	/*za svakih 10KM kupcu se dodaje 1 bod, pa ce tako kupovina od 128KM kupcu donijeti 12 bodova*/
	//denis->DodajKupovinu(128, zadovoljstvoKupca);
		if (iznosBodova >= 10) {
			_bodovi.push_back(iznosBodova / 10);
		}
		int zadnjiBodovi = _bodovi[_bodovi.size() - 1];
		if (zadnjiBodovi >= 5) {
			this_thread::sleep_for(chrono::seconds(3));
			thread t1(&Kupac::SaljiMail, this, iznosBodova / 10, zadnjiBodovi + (iznosBodova / 10));
			t1.join();
		}
	}
	void SacuvajBodove() {
		string adresa = _emailAdresa;
		adresa += ".txt";
		ofstream fajl(adresa);
		if (!fajl.fail()) {
			for (int i = 0; i < _bodovi.size(); i++)
			{
				fajl << _bodovi[i];
				if (i != _bodovi.size() - 1)
					fajl << endl;
			}
		}
		fajl.close();
	}
	void UcitajBodove() {
		/*string adresa = _emailAdresa;
		adresa += " txt";
		ifstream fajl(adresa);
		const int max = 20;
		char red[max];                
		string brojstr;
		int broj;
		while (!fajl.eof()) {
			fajl.getline(red,max);
			brojstr = red;
			broj=stoi(brojstr);
			_bodovi.push_back(broj);
			if (brojstr == "") {
				broj = stoi(brojstr);
				_bodovi.push_back(broj);
			}
		}fajl.close();*/
		                                 /// OVO MI NE RADI 
		cout << " sta bilo " << endl;
	}
	friend ostream& operator<< (ostream& COUT,  Kupac& obj) {
		COUT << crt << obj._imePrezime << "  ";
		COUT << obj._datumRodjenja << "  ";
		COUT<< obj._emailAdresa << " " << endl;
		COUT << "KUPOVINE -> " << crt;
		for (size_t i = 0; i < obj._kupovine->getTrenutno(); i++)
			COUT << "Iznos racuna: " << obj._kupovine->getElement1(i) << "KM, zadovoljstvo kupca: " << obj._kupovine->getElement2(i) << crt;
		COUT << "BODOVI -> ";
		for (size_t i = 0; i < obj._bodovi.size(); i++)
			COUT << obj._bodovi[i] << ", ";
		COUT << crt;
		return COUT;
	}
	
	Rjecnik<Karakteristike, const char*> GetKupovineByKomentar(const char* rijec) {
		Rjecnik<Karakteristike, const char*> novi;
		string trazeni = rijec;
		for (int i = 0; i < _kupovine->getTrenutno(); i++)
		{
			for (int j = 0; j < _kupovine->getElement2(i).GetKomentareKarakteristika().getTrenutno(); j++)
			{
				string tekst = _kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j);
				if (tekst.find( trazeni) != -1)
					novi.AddElement(_kupovine->getElement2(i).GetKomentareKarakteristika().getElement1(j), _kupovine->getElement2(i).GetKomentareKarakteristika().getElement2(j));

			}
		}return novi;
	}
};



void main() {

	

	const int rjecnikTestSize = 9;
	Rjecnik<int, int> rjecnik1(false);
	for (int i = 0; i < rjecnikTestSize - 1; i++)
		rjecnik1.AddElement(i, i);//dodaje vrijednosti u rjecnik

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 ili T2), metoda AddElement treba baciti objekat tipa exception
		rjecnik1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	rjecnik1.AddElement(9, 9);

	cout << rjecnik1 << crt;

	/*
	* metoda prihvata indekse lokacije, te vraća elemente koji se u rjecniku nalaze na proslijedjenim lokacijama (ukljucujuci i te lokacije)
	*/
	Rjecnik<int, int> rjecnik2 = rjecnik1(2, 5);
	/*clanovi objekta rjecnik2 bi trebali biti:
	2 2
	3 3
	4 4
	5 5
	*/
	cout << rjecnik2 << crt;


	const int maxKupaca = 3;
	Osoba* kupci[maxKupaca];
	kupci[0] = new Kupac("Denis Music", Datum(12, 1, 1980), "denis@fit.ba");
	kupci[1] = new Kupac("Jasmin Azemovic", Datum(12, 2, 1980), "jasmin@fit.ba");
	kupci[2] = new Kupac("Adel Handzic", Datum(12, 3, 1980), "adel@edu.fit.ba");

	ZadovoljstvoKupca zadovoljstvoKupca(7);
	zadovoljstvoKupca.DodajKomentarKarakteristike(NARUDZBA, "Nismo mogli odabrati sve potrebne opcije");
	zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");

	try {
		//karakteristika kvalitet je vec komentarisana
		zadovoljstvoKupca.DodajKomentarKarakteristike(KVALITET, "Kvalitet je ocekivan");
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	zadovoljstvoKupca.DodajKomentarKarakteristike(PAKOVANJE, "Pakovanje je bio osteceno");
	zadovoljstvoKupca.DodajKomentarKarakteristike(ISPORUKA, "Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana");


	Kupac* denis = dynamic_cast<Kupac*>(kupci[0]);
	/*za svakih 10KM kupcu se dodaje 1 bod, pa ce tako kupovina od 128KM kupcu donijeti 12 bodova*/
	denis->DodajKupovinu(128, zadovoljstvoKupca);
	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();//12 bodova


	ZadovoljstvoKupca zadovoljstvoKupca2(4);
	zadovoljstvoKupca2.DodajKomentarKarakteristike(KVALITET, "Jako lose, proizvod ostecen");
	denis->DodajKupovinu(81, zadovoljstvoKupca2);

	cout << "Ukupno bodova -> " << denis->GetBodoviUkupno();//20 bodova

	/*prilikom svake kupovine, ukoliko je kupac ostvario vise od 5 bodova, potrebno je, u zasebnom thread-u (nakon 3 sekunde), poslati email sa sljedecim sadrzajem:

	  To: denis@fit.ba;
	  Subject: Osvareni bodovi

	  Postovani,

	  Prilikom posljednje kupovine ste ostvarili 8 bodova, tako da trenutno vas ukupan broj bodova iznosi 20.

	  Zahvaljujemo vam na kupovini.
	  Puno pozdrava
  */


  //ispisuje sve podatke o kupcu i njegovim kupovinama.
	denis->Info();
	/* Primjer ispisa:
		-------------------------------------------
		Denis Music 12.1.1980 denis@fit.ba
		KUPOVINE ->
		-------------------------------------------
		Iznos racuna: 128KM, zadovoljstvo kupca: 7
				NARUDZBA - Nismo mogli odabrati sve potrebne opcije
				KVALITET - Kvalitet je ocekivan
				PAKOVANJE - Pakovanje je bio osteceno
				ISPORUKA - Mada su najavili da ce proizvod biti isporucen u roku od 2 dana, cekali smo 5 dana
		-------------------------------------------
		Iznos racuna: 81KM, zadovoljstvo kupca: 4
				KVALITET - Jako lose, proizvod ostecen
		-------------------------------------------
		BODOVI -> 12, 8,
		-------------------------------------------
   */
   /*metoda SacuvajBodove treba sve clanove vector-a _bodovi upisati u fajl(ignorisuci ranije dodate/postojece vrijednosti u fajlu) pod nazivom emailKorisnika.txt npr. denis@fit.ba.txt.
   na osnovu trenutnog stanja objekta, sadrzaj fajla denis@fit.ba.txt bi trebao biti sljedeci:
   12
   8

   nakon spasavanja u fajl, sadrzaj vector-a ostaje nepromijenjen.
   */
	denis->SacuvajBodove();
	//metoda UcitajBodove ucitava sadrzaj fajla pod nazivom emailKorisnika.txt i njegove vrijednosti pohranjuje/dodaje u vector _bodovi (zadrzavajuci postojece bodove).  
	denis->UcitajBodove();

	//metoda GetKupovineByKomentar treba da pronadje i vrati sve karakteristike proizvoda i komentare kupaca koji sadrze vrijednost proslijedjenog parametra
	Rjecnik<Karakteristike, const char*> osteceniProizvodi = denis->GetKupovineByKomentar("ostecen");
	cout << crt << "Rezultat pretrage -> " << crt << osteceniProizvodi << crt;

	/*Ocekivani ispis:
		-------------------------------------------
		Rezultat pretrage ->
		-------------------------------------------
		PAKOVANJE Pakovanje je bio osteceno
		KVALITET Jako lose, proizvod ostecen
		-------------------------------------------
	*/

	for (size_t i = 0; i < maxKupaca; i++)
		delete kupci[i], kupci[i] = nullptr;

	cin.get();
	system("pause>0");
}