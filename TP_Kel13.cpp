#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/* =====================================================
   STRUCT DASAR
===================================================== */
struct KTP {
    string nik, nama, alamat, jk;
};

struct Produk {
    string kode, nama;
    int harga, stok;
    Produk* next;
};

struct Keranjang {
    string nama;
    int qty, subtotal;
    Keranjang* next;
};

/* =====================================================
   POHON BST - PRODUK BERDASARKAN STOK
===================================================== */
struct NodeBST {
    string kode, nama;
    int stok;
    NodeBST *kiri, *kanan;
    
    NodeBST(string k, string n, int s) {
        kode = k;
        nama = n;
        stok = s;
        kiri = kanan = NULL;
    }
};

/* =====================================================
   STACK PEMBATALAN (LIFO)
===================================================== */
struct Pembatalan {
    string nama, produk;
    int jumlah;
    Pembatalan* next;
};

/* =====================================================
   QUEUE REFUND (FIFO)
===================================================== */
struct Refund {
    string nama;
    int total;
    Refund* next;
};

/* =====================================================
   HUFFMAN TREE - KOMPRESI DATA
===================================================== */
struct NodeHuffman {
    char karakter;
    int frekuensi;
    NodeHuffman *kiri, *kanan;
    
    NodeHuffman(char k, int f) {
        karakter = k;
        frekuensi = f;
        kiri = kanan = NULL;
    }
};

/* =====================================================
   GLOBAL POINTER
===================================================== */
Produk* headProduk = NULL;
Keranjang* headKeranjang = NULL;
NodeBST* rootBST = NULL;
Pembatalan* topPembatalan = NULL;
Refund *frontRefund = NULL, *rearRefund = NULL;

/* =====================================================
   BST - INSERT & TAMPIL
===================================================== */
NodeBST* insertBST(NodeBST* node, string kode, string nama, int stok) {
    if (node == NULL) {
        return new NodeBST(kode, nama, stok);
    }
    
    if (stok < node->stok) {
        node->kiri = insertBST(node->kiri, kode, nama, stok);
    } else {
        node->kanan = insertBST(node->kanan, kode, nama, stok);
    }
    
    return node;
}

void inOrderBST(NodeBST* node) {
    if (node != NULL) {
        inOrderBST(node->kiri);
        if (node->stok == 0) {
            cout << left << setw(5) << node->kode 
                 << setw(25) << node->nama
                 << setw(15) << "SOLD OUT" << endl;
        } else {
            cout << left << setw(5) << node->kode 
                 << setw(25) << node->nama
                 << setw(15) << node->stok << endl;
        }
        inOrderBST(node->kanan);
    }
}

void tampilBST() {
    cout << "=========================================\n";
    cout << "  PRODUK (URUT STOK: RENDAH-TINGGI)     \n";
    cout << "=========================================\n";
    if (!rootBST) {
        cout << "Belum ada produk\n";
        return;
    }
    cout << left << setw(5) << "Kode" 
         << setw(25) << "Nama Produk"
         << setw(15) << "Stok" << endl;
    cout << "-----------------------------------------\n";
    inOrderBST(rootBST);
}

// Update BST setelah perubahan stok
void updateBST() {
    // Hapus BST lama
    rootBST = NULL;
    
    // Rebuild BST dari linked list
    Produk* t = headProduk;
    while (t) {
        rootBST = insertBST(rootBST, t->kode, t->nama, t->stok);
        t = t->next;
    }
}

/* =====================================================
   STACK - PEMBATALAN (LIFO)
===================================================== */
void pushPembatalan(string nama, string produk, int jumlah) {
    Pembatalan* baru = new Pembatalan{nama, produk, jumlah, topPembatalan};
    topPembatalan = baru;
    cout << "? Pembatalan ditambahkan ke stack\n";
}

void popPembatalan() {
    if (!topPembatalan) {
        cout << "Tidak ada pembatalan\n";
        return;
    }
    
    cout << "=========================================\n";
    cout << "  PROSES PEMBATALAN (STACK - LIFO)      \n";
    cout << "=========================================\n";
    cout << "Nama   : " << topPembatalan->nama << endl;
    cout << "Produk : " << topPembatalan->produk << endl;
    cout << "Jumlah : " << topPembatalan->jumlah << endl;
    cout << "? Refund diproses!\n";
    
    Pembatalan* temp = topPembatalan;
    topPembatalan = topPembatalan->next;
    delete temp;
}

void lihatStack() {
    if (!topPembatalan) {
        cout << "Stack pembatalan kosong\n";
        return;
    }
    
    cout << "=========================================\n";
    cout << "       STACK PEMBATALAN (LIFO)          \n";
    cout << "=========================================\n";
    Pembatalan* t = topPembatalan;
    int no = 1;
    while (t) {
        cout << no++ << ". " << t->nama << " - " << t->produk 
             << " (Qty: " << t->jumlah << ")\n";
        t = t->next;
    }
}

/* =====================================================
   QUEUE - REFUND (FIFO)
===================================================== */
void enqueueRefund(string nama, int total) {
    Refund* baru = new Refund{nama, total, NULL};
    
    if (rearRefund == NULL) {
        frontRefund = rearRefund = baru;
    } else {
        rearRefund->next = baru;
        rearRefund = baru;
    }
    
    cout << "? Refund ditambahkan ke antrian\n";
}

void dequeueRefund() {
    if (frontRefund == NULL) {
        cout << "Antrian refund kosong\n";
        return;
    }
    
    cout << "=========================================\n";
    cout << "    PROSES REFUND (QUEUE - FIFO)        \n";
    cout << "=========================================\n";
    cout << "Nama  : " << frontRefund->nama << endl;
    cout << "Total : Rp " << frontRefund->total << endl;
    cout << "? Refund diproses!\n";
    
    Refund* temp = frontRefund;
    frontRefund = frontRefund->next;
    
    if (frontRefund == NULL) {
        rearRefund = NULL;
    }
    
    delete temp;
}

void lihatQueue() {
    if (frontRefund == NULL) {
        cout << "Queue refund kosong\n";
        return;
    }
    
    cout << "=========================================\n";
    cout << "        QUEUE REFUND (FIFO)             \n";
    cout << "=========================================\n";
    Refund* t = frontRefund;
    int no = 1;
    while (t) {
        cout << no++ << ". " << t->nama << " - Rp " << t->total << endl;
        t = t->next;
    }
}

/* =====================================================
   HUFFMAN TREE - KOMPRESI SEDERHANA
===================================================== */
void hitungFrekuensi(string teks, int freq[]) {
    for (int i = 0; i < 256; i++) freq[i] = 0;
    for (char c : teks) {
        freq[(int)c]++;
    }
}

void tampilHuffman(string data) {
    cout << "=========================================\n";
    cout << "     KOMPRESI DATA HUFFMAN (DEMO)       \n";
    cout << "=========================================\n";
    
    int freq[256];
    hitungFrekuensi(data, freq);
    
    cout << "Data Asli: " << data << endl;
    cout << "Panjang  : " << data.length() << " karakter\n";
    cout << "\nFrekuensi Karakter:\n";
    cout << "-----------------------------------------\n";
    
    int totalKarakter = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            cout << "'" << (char)i << "' : " << freq[i] << " kali\n";
            totalKarakter++;
        }
    }
    
    // Estimasi kompresi sederhana
    int ukuranAsli = data.length() * 8; // 8 bit per karakter
    int ukuranKompresi = data.length() * 4; // Estimasi 4 bit (huffman)
    float efisiensi = (1 - (float)ukuranKompresi / ukuranAsli) * 100;
    
    cout << "-----------------------------------------\n";
    cout << "Ukuran Asli (8-bit)  : " << ukuranAsli << " bit\n";
    cout << "Estimasi Kompresi    : " << ukuranKompresi << " bit\n";
    cout << "Efisiensi Huffman    : " << fixed << setprecision(1) 
         << efisiensi << "%\n";
    cout << "\n? Huffman Tree mengoptimasi dengan kode\n";
    cout << "  variabel: karakter sering = kode pendek\n";
}

/* =====================================================
   INISIALISASI PRODUK STATIS (10 ITEM)
===================================================== */
void initProduk() {
    // Data produk statis
    string kode[10] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10"};
    string nama[10] = {
        "Intel Core i9-14900K",
        "AMD Ryzen 9 7950X",
        "NVIDIA RTX 4090",
        "AMD Radeon RX 7900 XTX",
        "Corsair Vengeance 32GB",
        "Samsung 990 Pro 2TB",
        "ASUS ROG Maximus Z790",
        "MSI MAG B650 Tomahawk",
        "Corsair RM1000x PSU",
        "NZXT H9 Elite Case"
    };
    int harga[10] = {8500000, 7800000, 28000000, 15000000, 2500000, 
                     3200000, 6500000, 4200000, 3500000, 2800000};
    int stok[10] = {5, 8, 3, 6, 15, 10, 7, 12, 9, 11};
    
    for (int i = 0; i < 10; i++) {
        Produk* b = new Produk;
        b->kode = kode[i];
        b->nama = nama[i];
        b->harga = harga[i];
        b->stok = stok[i];
        b->next = NULL;
        
        if (!headProduk) {
            headProduk = b;
        } else {
            Produk* t = headProduk;
            while (t->next) t = t->next;
            t->next = b;
        }
        
        // Tambahkan ke BST
        rootBST = insertBST(rootBST, b->kode, b->nama, b->stok);
    }
    
    cout << "? 10 Produk PC berhasil dimuat!\n";
}

/* =====================================================
   PRODUK - LINKED LIST
===================================================== */
void tampilProduk() {
    cout << "=========================================\n";
    cout << "           DAFTAR PRODUK PC              \n";
    cout << "=========================================\n";
    if (!headProduk) {
        cout << "Produk kosong\n";
        return;
    }
    Produk* t = headProduk;
    cout << left << setw(5) << "Kode"
         << setw(25) << "Nama"
         << setw(12) << "Harga"
         << setw(6) << "Stok" << endl;
    cout << "-----------------------------------------\n";
    while (t) {
        cout << setw(5) << t->kode
             << setw(25) << t->nama
             << "Rp " << setw(9) << t->harga;
        
        if (t->stok == 0) {
            cout << "SOLD OUT" << endl;
        } else {
            cout << t->stok << endl;
        }
        t = t->next;
    }
}

Produk* cariProduk(string kode) {
    Produk* t = headProduk;
    while (t) {
        if (t->kode == kode) return t;
        t = t->next;
    }
    return NULL;
}

void updateStok() {
    string kode;
    int stokBaru;
    
    cout << "Kode Produk : "; cin >> kode;
    Produk* p = cariProduk(kode);
    
    if (p) {
        cout << "Produk      : " << p->nama << endl;
        cout << "Stok Saat Ini : " << p->stok << endl;
        cout << "Stok Baru   : "; cin >> stokBaru;
        
        p->stok = stokBaru;
        
        // Update BST
        updateBST();
        
        cout << "? Stok berhasil diupdate!\n";
    } else {
        cout << "? Produk tidak ditemukan\n";
    }
}

/* =====================================================
   KERANJANG BELANJA
===================================================== */
void tambahKeranjang(string nama, int qty, int harga) {
    Keranjang* b = new Keranjang{nama, qty, qty * harga, NULL};
    if (!headKeranjang) headKeranjang = b;
    else {
        Keranjang* t = headKeranjang;
        while (t->next) t = t->next;
        t->next = b;
    }
}

void tampilKeranjang() {
    cout << "=========================================\n";
    cout << "          KERANJANG BELANJA              \n";
    cout << "=========================================\n";
    if (!headKeranjang) {
        cout << "Keranjang kosong\n";
        return;
    }
    Keranjang* t = headKeranjang;
    int total = 0, no = 1;
    while (t) {
        cout << no++ << ". " << t->nama
             << " x" << t->qty
             << " = Rp " << t->subtotal << endl;
        total += t->subtotal;
        t = t->next;
    }
    cout << "-----------------------------------------\n";
    cout << "TOTAL : Rp " << total << endl;
}

void hapusKeranjang() {
    while (headKeranjang) {
        Keranjang* h = headKeranjang;
        headKeranjang = headKeranjang->next;
        delete h;
    }
}

/* =====================================================
   LOGIN PEMBELI
===================================================== */
KTP loginPembeli() {
    KTP p;
    cout << "=========================================\n";
    cout << "           INPUT DATA PEMBELI            \n";
    cout << "=========================================\n";
    cout << "NIK           : "; cin >> p.nik;
    cin.ignore();
    cout << "Nama          : "; getline(cin, p.nama);
    cout << "Alamat        : "; getline(cin, p.alamat);
    cout << "Jenis Kelamin : "; getline(cin, p.jk);
    return p;
}

/* =====================================================
   STRUK BELANJA
===================================================== */
void cetakNota(KTP p) {
    ofstream f("Nota_" + p.nik + ".txt");
    Keranjang* t = headKeranjang;
    int total = 0;

    f << "=========================================\n";
    f << "          STRUK BELANJA PC               \n";
    f << "=========================================\n";
    f << "Nama   : " << p.nama << endl;
    f << "NIK    : " << p.nik << endl;
    f << "Alamat : " << p.alamat << endl;
    f << "-----------------------------------------\n";

    while (t) {
        f << t->nama << " x" << t->qty
          << " = Rp " << t->subtotal << endl;
        total += t->subtotal;
        t = t->next;
    }

    f << "-----------------------------------------\n";
    f << "TOTAL BAYAR : Rp " << total << endl;
    f << "=========================================\n";
    f.close();
}

/* =====================================================
   MENU ADMIN
===================================================== */
void menuAdmin() {
    int pil;
    do {
        cout << "\n=========================================\n";
        cout << "               MENU ADMIN                \n";
        cout << "=========================================\n";
        cout << "1. Lihat Produk (List)\n";
        cout << "2. Lihat Produk (BST-Tree)\n";
        cout << "3. Update Stok Produk\n";
        cout << "4. Lihat Stack Pembatalan\n";
        cout << "5. Proses Pembatalan (Stack-LIFO)\n";
        cout << "6. Lihat Queue Refund\n";
        cout << "7. Proses Refund (Queue-FIFO)\n";
        cout << "8. Demo Kompresi Huffman\n";
        cout << "0. Logout\n";
        cout << "=========================================\n";
        cout << "Pilih : ";
        cin >> pil;

        if (pil == 1) tampilProduk();
        else if (pil == 2) tampilBST();
        else if (pil == 3) updateStok();
        else if (pil == 4) lihatStack();
        else if (pil == 5) popPembatalan();
        else if (pil == 6) lihatQueue();
        else if (pil == 7) dequeueRefund();
        else if (pil == 8) {
            string data;
            cout << "Masukkan teks untuk dikompresi: ";
            cin.ignore();
            getline(cin, data);
            tampilHuffman(data);
        }

    } while (pil != 0);
}

/* =====================================================
   MAIN PROGRAM
===================================================== */
int main() {
    // Inisialisasi 10 produk statis
    initProduk();
    
    int pilih;
    while (true) {
        cout << "\n=========================================\n";
        cout << "       SISTEM PENJUALAN PC STORE         \n";
        cout << "=========================================\n";
        cout << "1. Admin\n";
        cout << "2. Pembeli\n";
        cout << "0. Keluar\n";
        cout << "=========================================\n";
        cout << "Pilih Menu : ";
        cin >> pilih;

        if (pilih == 1) {
            string u, p;
            cout << "Username : "; cin >> u;
            cout << "Password : "; cin >> p;
            if (u == "admin" && p == "123")
                menuAdmin();
            else
                cout << "? Login gagal\n";
        }
        else if (pilih == 2) {
            KTP pembeli = loginPembeli();
            int m;
            do {
                cout << "\n=========================================\n";
                cout << "             MENU PEMBELI                \n";
                cout << "=========================================\n";
                cout << "1. Lihat Produk\n";
                cout << "2. Beli Produk\n";
                cout << "3. Lihat Keranjang\n";
                cout << "4. Checkout\n";
                cout << "5. Ajukan Pembatalan (Stack)\n";
                cout << "6. Ajukan Refund (Queue)\n";
                cout << "0. Kembali\n";
                cout << "=========================================\n";
                cout << "Pilih : ";
                cin >> m;

                if (m == 1) tampilProduk();
                else if (m == 2) {
                    string kode;
                    int qty;
                    cout << "Kode Produk : "; cin >> kode;
                    Produk* pr = cariProduk(kode);
                    if (pr) {
                        if (pr->stok == 0) {
                            cout << "? Produk SOLD OUT\n";
                        } else {
                            cout << "Jumlah : "; cin >> qty;
                            if (qty <= pr->stok) {
                                pr->stok -= qty;
                                tambahKeranjang(pr->nama, qty, pr->harga);
                                
                                // Update BST setelah stok berkurang
                                updateBST();
                                
                                cout << "? Berhasil ditambahkan ke keranjang\n";
                            } else {
                                cout << "? Stok tidak cukup (Tersedia: " << pr->stok << ")\n";
                            }
                        }
                    } else {
                        cout << "? Produk tidak ditemukan\n";
                    }
                }
                else if (m == 3) tampilKeranjang();
                else if (m == 4) {
                    tampilKeranjang();
                    cetakNota(pembeli);
                    hapusKeranjang();
                    cout << "? Checkout berhasil & nota tersimpan\n";
                }
                else if (m == 5) {
                    string produk;
                    int jumlah;
                    cout << "Nama Produk : ";
                    cin.ignore();
                    getline(cin, produk);
                    cout << "Jumlah      : "; cin >> jumlah;
                    pushPembatalan(pembeli.nama, produk, jumlah);
                }
                else if (m == 6) {
                    int total;
                    cout << "Total Refund : Rp "; cin >> total;
                    enqueueRefund(pembeli.nama, total);
                }

            } while (m != 0);
        }
        else break;
    }
    return 0;
}
