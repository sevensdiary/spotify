#include <iostream>
#include <fstream>
#include <string>
#include <climits>  // untuk INT_MAX
using namespace std;

// daftar playlist nyimpen kumpulan playlist, nah playlist itu kumpulan lagu-lagu
// lagu nyimpen data 1 lagu dan pointer ke lagu berikutnya
// playlist nyimpen nama playlist, daftar lagu dan sambungin ke playlist lainnya
struct Lagu {
    string judul;
    string artis;
    string genre;
    int durasi;
    Lagu* next; //pointer buat nyambungin ke lagu berikutnya
};

struct Playlist {
    string nama; //nyimpen nama playlist
    Lagu* head; //pointer ke lagu pertama dlm playlist itu
    Playlist* next; //pointer buat nyambungin ke playlist berikutnya
};

Playlist* daftarPlaylist = nullptr; // daftarPlaylist itu pointer bertipe Playlist. dia bs nunjuk ke objectnya Playlist

void menuUtama();
void buatPlaylist();
void lihatDaftarPlaylist();
void tampilkanMenuPlaylist(Playlist*);
Playlist* cariPlaylist(const string&);
void tambahLagu(Playlist*);
void tampilkanPlaylist(Playlist*);
void tampilkanStatistik(Playlist*);
void hapusLagu(Playlist*);
void hapusPlaylist(const string&);
void renamePlaylist(Playlist*);
void simpanKeFile();
void bacaDariFile();
void hapusSemuaPlaylist();
void urutkanLagu(Playlist* pl);
void tampilkanRecap();

int main() {
    bacaDariFile();
    menuUtama();
    hapusSemuaPlaylist();
    return 0;
}

void menuUtama() {
    int pilihan;
    do {
        cout << "\n------------------------" << endl;
        cout << "         Spotifay         " << endl;
        cout << "------------------------" << endl;
        cout << "1. Buat Playlist" << endl;
        cout << "2. Lihat Playlist" << endl;
        cout << "3. Recap" << endl;
        cout << "0. Keluar" << endl;
        cout << "> Pilih Menu : ";
        while (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input harus angka!\n";
        }
        cin.ignore();

        switch (pilihan) {
            case 1: buatPlaylist();        break;
            case 2: lihatDaftarPlaylist(); break;
            case 3: tampilkanRecap();      break;
            case 0:
                simpanKeFile();
                cout << "Data tersimpan. Sampai jumpa!\n";
                break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);
}

void buatPlaylist() {
    string nama;
    cout << "\n--- Buat Playlist ---\n";
    cout << "Nama playlist: ";getline(cin, nama);

    //validasi nama playlist kodong
    if(nama.empty()){
        //hitung jumlah playlistnya
        int count = 1;
        Playlist* temp = daftarPlaylist;
        while(temp){
            count++;
            temp = temp->next;
        }
        nama = "Playlist ke-" + to_string(count);
        cout << "Nama otomatis: [ " << nama << " ]" << endl;
    }

    // Cek duplikat nama
    if (cariPlaylist(nama)) {
        cout << "Playlist [" << nama << "] sudah ada!\n";
        return;
    }

    Playlist* baru  = new Playlist{nama, nullptr, nullptr}; //membuat variabel pointer bernama baru dgn tipe struct playlist
    baru->next      = daftarPlaylist;
    daftarPlaylist  = baru;

    cout << "Playlist [" << nama << "] berhasil dibuat!\n";
    simpanKeFile();
    tampilkanMenuPlaylist(baru);
}

void lihatDaftarPlaylist() {
    cout << "\n--- Daftar Playlist ---\n";
    if (!daftarPlaylist) {
        cout << "Belum ada playlist.\n";
        return;
    }

    Playlist* temp = daftarPlaylist;
    int i = 1;
    while (temp) {
        // Hitung jumlah lagu
        int jml = 0;
        Lagu* l = temp->head;
        while (l) { jml++; l = l->next; }
        cout << i++ << ". " << temp->nama << " (" << jml << " lagu)\n";
        temp = temp->next;
    }

    cout << "\nMasukkan nama playlist (kosong = kembali): ";
    string nama;
    getline(cin, nama);
    if (!nama.empty()) {
        Playlist* p = cariPlaylist(nama);
        if (p) tampilkanMenuPlaylist(p);
        else    cout << "Playlist tidak ditemukan.\n";
    }
}

void tampilkanMenuPlaylist(Playlist* pl) {
    int pilihan;
    do {
        cout << "\n--- Playlist [" << pl->nama << "] ---\n";
        cout << "1. Lihat isi playlist\n";
        cout << "2. Tambah lagu\n";
        cout << "3. Urutkan lagu (durasi pendek -> panjang)\n";
        cout << "4. Hapus lagu\n";
        cout << "5. Statistik playlist\n";
        cout << "6. Rename Playlist\n";
        cout << "7. Hapus playlist ini\n";
        cout << "0. Kembali\n";
        cout << "> Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: tampilkanPlaylist(pl);   break;
            case 2: tambahLagu(pl);          break;
            case 3: urutkanLagu(pl);         break;
            case 4: hapusLagu(pl);           break;
            case 5: tampilkanStatistik(pl);  break;
            case 6: renamePlaylist(pl);      break;
            case 7:{
                char konfirmasi;
                cout << "Yakin hapus playlist[" << pl->nama << "] (y/n): ";cin>>konfirmasi;cin.ignore();
                if(konfirmasi!='y' && konfirmasi!='Y'){
                    cout << "Dibatalkan.\n" << endl;
                    break;
                }

                hapusPlaylist(pl->nama);
                return;  // playlist sudah dihapus, keluar dari fungsi
            }
            case 0: break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);
}

Playlist* cariPlaylist(const string& nama) {
    Playlist* temp = daftarPlaylist;
    while (temp) {
        if (temp->nama == nama) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void tambahLagu(Playlist* pl) {
    string judul, artis, genre, sdurasi;
    cout << "\n--- Tambah Lagu ---\n";
    cout << "Judul : "; getline(cin, judul);
    cout << "Artis : "; getline(cin, artis);
    cout << "Genre : "; getline(cin, genre);

    int durasi;
    while (true) {
        cout << "Durasi (detik): ";
        getline(cin, sdurasi);
        bool valid = !sdurasi.empty();
        for (char c : sdurasi) if (!isdigit(c)) { valid = false; break; }
        if (!valid) { cout << "Input harus berupa angka positif!\n"; continue; }
        durasi = stoi(sdurasi);
        break;
    }

    Lagu* baru = new Lagu{judul, artis, genre, durasi, nullptr};
    if (!pl->head) {
        pl->head = baru;
    } else {
        Lagu* temp = pl->head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
    cout << "Lagu [" << judul << "] berhasil ditambahkan.\n";
    simpanKeFile();
}

void tampilkanPlaylist(Playlist* pl) {
    cout << "\n--- Isi Playlist [" << pl->nama << "] ---\n";
    if (!pl->head) { cout << "Playlist kosong.\n"; return; }

    Lagu* temp = pl->head;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->judul << " - " << temp->artis
             << " [" << temp->genre << "] (" << temp->durasi << " detik)\n";
        temp = temp->next;
    }
}

void renamePlaylist(Playlist* pl) {
    string namaBaru;
    cout << "Nama baru (kosong = batal): ";
    getline(cin, namaBaru);

    if (namaBaru.empty()) { cout << "Dibatalkan.\n"; return; }
    if (cariPlaylist(namaBaru)) { cout << "Nama [" << namaBaru << "] sudah ada!\n"; return; }

    pl->nama = namaBaru;
    cout << "Playlist berhasil direname jadi [" << namaBaru << "].\n";
    simpanKeFile();
}

void urutkanLagu(Playlist* pl) {
    if (!pl->head || !pl->head->next) {
        cout << "Lagu kurang dari 2, tidak perlu diurutkan.\n";
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Lagu* curr = pl->head;
        Lagu* prev = nullptr;
        Lagu* next = nullptr;

        while (curr->next) {
            next = curr->next;
            if (curr->durasi > next->durasi) {
                if (prev) prev->next = next;
                else       pl->head  = next;
                curr->next = next->next;
                next->next = curr;
                swapped = true;
                prev    = next;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (swapped);

    cout << "Berhasil diurutkan (pendek -> panjang).\n";
    tampilkanPlaylist(pl);
    simpanKeFile();
}

void hapusLagu(Playlist* pl) {
    tampilkanPlaylist(pl);
    if (!pl->head) return;

    cout << "Judul lagu yang ingin dihapus: ";
    string judul;
    getline(cin, judul);

    Lagu* temp = pl->head;
    Lagu* prev = nullptr;
    while (temp && temp->judul != judul) { 
        prev = temp; 
        temp = temp->next; 
    }

    if (!temp) { cout << "Lagu tidak ditemukan.\n"; return; }
    char konfirmasi;
    cout << "Yakin hapus [" << judul << "]? (y/n): ";cin>>konfirmasi;
    cin.ignore();
    if(konfirmasi!= 'y' && konfirmasi != 'Y') {
        cout << "Dibatalkan!\n"; 
        return;
    } 
    
    if (!prev) pl->head    = temp->next;
    else       prev->next  = temp->next;
    delete temp;
    cout << "Lagu [" << judul << "] berhasil dihapus.\n";
    simpanKeFile();
}

void hapusPlaylist(const string& nama) {
    Playlist* temp = daftarPlaylist;
    Playlist* prev = nullptr;
    while (temp && temp->nama != nama) { prev = temp; temp = temp->next; }
    if (!temp) { cout << "Playlist tidak ditemukan.\n"; return; }

    // Hapus semua lagu di playlist ini
    while (temp->head) {
        Lagu* hapus = temp->head;
        temp->head  = temp->head->next;
        delete hapus;
    }
    if (!prev) daftarPlaylist = temp->next;
    else       prev->next     = temp->next;
    delete temp;
    cout << "Playlist [" << nama << "] berhasil dihapus.\n";
    simpanKeFile();
}

void tampilkanStatistik(Playlist* pl) {
    cout << "\n--- Statistik [" << pl->nama << "] ---\n";
    if (!pl->head) { cout << "Playlist kosong.\n"; return; }

    int total = 0, totalDurasi = 0;
    int terpendek = INT_MAX, terpanjang = 0;
    string judulPendek, judulPanjang;
    Lagu* temp = pl->head;

    while (temp) {
        total++;
        totalDurasi += temp->durasi;
        if (temp->durasi < terpendek) { terpendek = temp->durasi; judulPendek = temp->judul; }
        if (temp->durasi > terpanjang) { terpanjang = temp->durasi; judulPanjang = temp->judul; }
        temp = temp->next;
    }

    cout << "Jumlah lagu      : " << total << "\n";
    cout << "Total durasi     : " << totalDurasi << " detik\n";
    cout << "Rata-rata durasi : " << totalDurasi / total << " detik\n";
    cout << "Lagu terpendek   : " << judulPendek << " (" << terpendek << " detik)\n";
    cout << "Lagu terpanjang  : " << judulPanjang << " (" << terpanjang << " detik)\n";
}

void simpanKeFile() {
    ofstream file("playlist.txt");
    Playlist* p = daftarPlaylist;
    while (p) {
        file << "#PLAYLIST;" << p->nama << "\n";
        Lagu* l = p->head;
        while (l) {
            file << l->judul << ";" << l->artis << ";" << l->genre << ";" << l->durasi << "\n";
            l = l->next;
        }
        file << "#END\n";
        p = p->next;
    }
    file.close();
}

void bacaDariFile() {
    ifstream file("playlist.txt"); //ifstream = input file stream atau membuka file playlist.txt
    if (!file.is_open()) return; //kalau file gagal dibuka, langsung keluar dari fungsi
    hapusSemuaPlaylist(); // hapus semua playlist lama di memori dulu, biar ga numpuk2 kl method ini dipanggil trs

    string line;
    Playlist* current = nullptr; //playlist yg sedang dibaca skrg
    while (getline(file, line)) {  // selama masih ada baris di file, ambil 1 baris masuk ke line
        if (line.find("#PLAYLIST;") == 0) {
            string nama = line.substr(10); //nama playlist mulai dr index ke 10
            current = new Playlist{nama, nullptr, daftarPlaylist};
            daftarPlaylist = current;
        } else if (line == "#END") {
            current = nullptr;
        } else if (current && !line.empty()) {
            size_t p1 = line.find(';');
            size_t p2 = line.find(';', p1 + 1);
            size_t p3 = line.find(';', p2 + 1);
            if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

            string judul = line.substr(0, p1);
            string artis = line.substr(p1 + 1, p2 - p1 - 1);
            string genre = line.substr(p2 + 1, p3 - p2 - 1);
            int durasi   = stoi(line.substr(p3 + 1));

            Lagu* baru = new Lagu{judul, artis, genre, durasi, nullptr};
            if (!current->head) {
                current->head = baru;
            } else {
                Lagu* temp = current->head;
                while (temp->next) temp = temp->next;
                temp->next = baru;
            }
        }
    }
    file.close();
}

void hapusSemuaPlaylist() {
    while (daftarPlaylist) {
        Playlist* p = daftarPlaylist;
        daftarPlaylist = daftarPlaylist->next;
        while (p->head) {
            Lagu* l = p->head;
            p->head = p->head->next;
            delete l;
        }
        delete p;
    }
}

void tampilkanRecap() {
    cout << "\n========== Recap ==========\n";
    if (!daftarPlaylist) { cout << "Belum ada playlist.\n"; return; }

    int totalPlaylist = 0, totalLagu = 0, durasiTotal = 0;
    int durasiTerpendek = INT_MAX, durasiTerpanjang = 0;
    string laguTerpendek, laguTerpanjang, playlistTerbanyak;
    int maxLagu = 0;

    Playlist* p = daftarPlaylist;
    while (p) {
        totalPlaylist++;
        int jumlahLagu = 0;
        Lagu* l = p->head;
        while (l) {
            jumlahLagu++; totalLagu++; durasiTotal += l->durasi;
            if (l->durasi < durasiTerpendek) { durasiTerpendek = l->durasi; laguTerpendek = l->judul + " - " + l->artis; }
            if (l->durasi > durasiTerpanjang) { durasiTerpanjang = l->durasi; laguTerpanjang = l->judul + " - " + l->artis; }
            l = l->next;
        }
        if (jumlahLagu > maxLagu) { maxLagu = jumlahLagu; playlistTerbanyak = p->nama; }
        p = p->next;
    }

    cout << "Jumlah Playlist       : " << totalPlaylist << "\n";
    cout << "Jumlah Lagu Total     : " << totalLagu << "\n";
    if (totalLagu > 0) {
        cout << "Rata-rata Durasi      : " << durasiTotal / totalLagu << " detik\n";
        cout << "Lagu Terpendek        : " << laguTerpendek << " (" << durasiTerpendek << " detik)\n";
        cout << "Lagu Terpanjang       : " << laguTerpanjang << " (" << durasiTerpanjang << " detik)\n";
        cout << "Playlist Terbanyak    : " << playlistTerbanyak << " (" << maxLagu << " lagu)\n";
    }
    cout << "===========================\n";
}
