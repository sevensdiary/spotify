#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Lagu {
    string judul, artis, genre;
    int durasi;
    Lagu* next;

    Lagu(string j, string a, string g, int d) {
        judul = j;
        artis = a;
        genre = g;
        durasi = d;
        next = nullptr;
    }
};

struct Playlist {
    string nama;       
    Lagu*  headLagu;
    Lagu*  tailLagu;    
    int    jumlahLagu;  
    Playlist* next;
    Playlist* prev;     

    Playlist(string n) {
        nama       = n;
        headLagu   = nullptr;
        tailLagu   = nullptr;
        jumlahLagu = 0;
        next       = nullptr;
        prev       = nullptr;
    }
};

Playlist* headPlaylist = nullptr;
Playlist* tailPlaylist = nullptr;
int totalPlaylist = 0;

void      menuUtama();
void      buatPlaylist();
void      lihatDaftarPlaylist();
void      tampilkanMenuPlaylist(Playlist* pl);
Playlist* cariPlaylist(const string& nama);
void      tambahLagu(Playlist* pl);
void      tampilkanPlaylist(Playlist* pl);
void      tampilkanStatistik(Playlist* pl);
void      hapusLagu(Playlist* pl);
void      hapusPlaylist(const string& nama);
void      simpanKeFile();
void      bacaDariFile();
void      hapusSemuaPlaylist();
void      urutkanLagu(Playlist* pl);
void      tampilkanRecap();

string formatDurasi(int detik) {
    int menit = detik / 60;
    int sisa = detik % 60;
    string hasil = to_string(menit) + ":";
    if (sisa < 10) hasil += "0";
    hasil += to_string(sisa);
    return hasil;
} 

void cetakGaris() {
    cout << "============================================================" << endl;
}

void cetakGarisPendek() {
    cout << "------------------------------------------------------------" << endl;
}

void menuUtama() {
    int pilihan;
    bacaDariFile();
    do {
        cout << endl;
        cetakGaris();
        cout << " SPOTIFY - Your Music Space " << endl;
        cetakGaris();
        cout << "  Total Playlist: " << totalPlaylist << endl;
        cetakGarisPendek();
        cout << "  [1] Buat Playlist Baru" << endl;
        cout << "  [2] Lihat Daftar Playlist" << endl;
        cout << "  [3] Cari & Buka Playlist" << endl;
        cout << "  [4] Hapus Playlist" << endl;
        cout << "  [5] Lihat Rekap Semua Playlist" << endl;
        cout << "  [6] Simpan ke File" << endl;
        cout << "  [7] Baca dari File" << endl;
        cout << "  [0] Keluar" << endl;
        cetakGaris();
        cout << "  Pilihan Anda: ";
        cin >> pilihan;
        cin.ignore(); 

        switch (pilihan) {
            case 1:
                buatPlaylist();
                break;
            case 2:
                lihatDaftarPlaylist();
                break;
            case 3: {
                string namaCari;
                cout << "\n  Masukkan nama playlist yang dicari: ";
                getline(cin, namaCari);
                Playlist* ditemukan = cariPlaylist(namaCari);
                if (ditemukan != nullptr) {
                    tampilkanMenuPlaylist(ditemukan);
                } else {
                    cout << "\n  [!] Playlist \"" << namaCari << "\" tidak ditemukan." << endl;
                }
                break;
            }
            case 4: {
                string namaHapus;
                cout << "\n  Masukkan nama playlist yang akan dihapus: ";
                getline(cin, namaHapus);
                hapusPlaylist(namaHapus);
                break;
            }
            case 5:
                tampilkanRecap();
                break;
            case 6:
                simpanKeFile();
                break;
            case 7:
                bacaDariFile();
                break;
            case 0:
                cout << "\n  Menyimpan data..." << endl;
                simpanKeFile();
                hapusSemuaPlaylist();
                cout << "  Terima kasih! Sampai jumpa!" << endl;
                break;
            default:
                cout << "\n  [!] Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
}

void buatPlaylist() {
    cout << endl;
    cetakGaris();
    cout << "BUAT PLAYLIST BARU" << endl;
    cetakGaris();

    string nama;
    cout << "  Masukkan nama playlist: ";
    getline(cin, nama);

    if (cariPlaylist(nama) != nullptr) {
        cout << "\n  [!] Playlist dengan nama \"" << nama << "\" sudah ada!" << endl;
        return;
    }

    Playlist* baru = new Playlist(nama);

    if (headPlaylist == nullptr) {
        headPlaylist = baru;
        tailPlaylist = baru;
    } else {
        baru->prev = tailPlaylist;  
        tailPlaylist->next = baru;          
        tailPlaylist = baru;          
    }

    totalPlaylist++;
    cout << "\n  [v] Playlist \"" << nama << "\" berhasil dibuat!" << endl;
}

void lihatDaftarPlaylist() {
    cout << endl;
    cetakGaris();
    cout << "DAFTAR PLAYLIST" << endl;
    cetakGaris();

    if (headPlaylist == nullptr) {
        cout << "(Belum ada playlist. Buat playlist baru terlebih dahulu!)" << endl;
        return;
    }

    Playlist* current = headPlaylist;
    int nomor = 1;

    while (current != nullptr) {
        cout << "[" << nomor << "] " << current->nama;
        cout << "(" << current->jumlahLagu << " lagu)" << endl;
        current = current->next; 
        nomor++;
    }

    cetakGaris();
    cout << "  Total: " << totalPlaylist << " playlist" << endl;
}

void tampilkanMenuPlaylist(Playlist* pl) {
    if (pl == nullptr) return;

    int pilihan;
    do {
        cout << endl;
        cetakGaris();
        cout << "PLAYLIST: " << pl->nama << endl;
        cout << "Jumlah Lagu: " << pl->jumlahLagu << endl;
        cetakGaris();
        cout << "  [1] Tampilkan Semua Lagu" << endl;
        cout << "  [2] Tambah Lagu" << endl;
        cout << "  [3] Hapus Lagu" << endl;
        cout << "  [4] Urutkan Lagu (A-Z)" << endl;
        cout << "  [5] Lihat Statistik Playlist" << endl;
        cout << "  [0] Kembali ke Menu Utama" << endl;
        cetakGaris();
        cout << "  Pilihan Anda: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: tampilkanPlaylist(pl);    break;
            case 2: tambahLagu(pl);           break;
            case 3: hapusLagu(pl);            break;
            case 4: urutkanLagu(pl);          break;
            case 5: tampilkanStatistik(pl);   break;
            case 0: break;
            default:
                cout << "\n  [!] Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
}

Playlist* cariPlaylist(const string& nama) {
    Playlist* current = headPlaylist; 
    while (current != nullptr) {
        if (current->nama == nama) {
            return current; 
        }
        current = current->next; 
    }
    return nullptr; 
}

void tambahLagu(Playlist* pl) {
    if (pl == nullptr) return;

    cout << endl;
    cetakGaris();
    cout << "   TAMBAH LAGU ke \"" << pl->nama << "\"" << endl;
    cetakGaris();

    string judul, artis, genre;
    int menit, detik;

    cout << "  Judul lagu  : ";
    getline(cin, judul);
    cout << "  Nama artis  : ";
    getline(cin, artis);
    cout << "  Genre       : ";
    getline(cin, genre);
    cout << "  Durasi (menit): ";
    cin >> menit;
    cout << "  Durasi (detik): ";
    cin >> detik;
    cin.ignore();

    int totalDetik = (menit * 60) + detik;

    Lagu* lagubaru = new Lagu(judul, artis, genre, totalDetik);

    if (pl->headLagu == nullptr) {
        pl->headLagu = lagubaru;
        pl->tailLagu = lagubaru;
    } else {
        pl->tailLagu->next = lagubaru; 
        pl->tailLagu       = lagubaru; 
    }

    pl->jumlahLagu++;
    cout << "\n  [v] Lagu \"" << judul << "\" berhasil ditambahkan!" << endl;
}
 
void tampilkanPlaylist(Playlist* pl) {
    if (pl == nullptr) return;

    cout << endl;
    cetakGaris();
    cout << "   PLAYLIST: " << pl->nama << endl;
    cetakGarisPendek();

    if (pl->headLagu == nullptr) {
        cout << "  (Playlist masih kosong!)" << endl;
        return;
    }

    Lagu* current = pl->headLagu;
    int nomor = 1;

    cout << "  No  Judul                  Artis              Genre       Durasi" << endl;
    cetakGarisPendek();

    while (current != nullptr) {
        cout << "  " << nomor << ".  ";
        cout << current->judul.substr(0, 20);
        // Padding agar rata
        for (int i = current->judul.length(); i < 22; i++) cout << " ";
        cout << current->artis.substr(0, 18);
        for (int i = current->artis.length(); i < 18; i++) cout << " ";
        cout << "  " << current->genre.substr(0, 10);
        for (int i = current->genre.length(); i < 10; i++) cout << " ";
        cout << "  " << formatDurasi(current->durasi) << endl;

        current = current->next;
        nomor++;
    }

    cetakGaris();
    cout << "  Total: " << pl->jumlahLagu << " lagu" << endl;
}

void tampilkanStatistik(Playlist* pl) {
    if (pl == nullptr) return;

    cout << endl;
    cetakGaris();
    cout << "   STATISTIK PLAYLIST: " << pl->nama << endl;
    cetakGaris();

    if (pl->headLagu == nullptr) {
        cout << "  (Playlist masih kosong!)" << endl;
        return;
    }

    int    totalDetik   = 0;
    int    maxDurasi    = 0;
    int    minDurasi    = pl->headLagu->durasi;
    string laguterpanjang = "";
    string laguterpendek  = "";

    Lagu* current = pl->headLagu;
    while (current != nullptr) {
        totalDetik += current->durasi;

        if (current->durasi > maxDurasi) {
            maxDurasi       = current->durasi;
            laguterpanjang  = current->judul;
        }
        if (current->durasi < minDurasi) {
            minDurasi      = current->durasi;
            laguterpendek  = current->judul;
        }

        current = current->next;
    }

    int rataRata = totalDetik / pl->jumlahLagu;

    cout << "  Nama Playlist    : " << pl->nama << endl;
    cout << "  Jumlah Lagu      : " << pl->jumlahLagu << " lagu" << endl;
    cout << "  Total Durasi     : " << formatDurasi(totalDetik) << endl;
    cout << "  Rata-rata Durasi : " << formatDurasi(rataRata) << endl;
    cout << "  Lagu Terpanjang  : " << laguterpanjang << " (" << formatDurasi(maxDurasi) << ")" << endl;
    cout << "  Lagu Terpendek   : " << laguterpendek  << " (" << formatDurasi(minDurasi) << ")" << endl;
    cetakGaris();
}
 