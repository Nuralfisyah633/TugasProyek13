#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// =========================================================
// 1. STRUCT KTP
// =========================================================
struct KTP {
    string nik, nama, alamat, jk;
};

// =========================================================
// 2. AVL TREE / BST (DATA PRODUK)
// =========================================================
struct Produk {
    string kode, nama;
    int harga, stok, height;
    Produk *left, *right;
};

Produk* rootProduk = NULL;

int height(Produk* n) { return n ? n->height : 0; }
int max(int a, int b) { return a > b ? a : b; }

Produk* rotasiKanan(Produk* y) {
    Produk* x = y->left;
    Produk* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Produk* rotasiKiri(Produk* x) {
    Produk* y = x->right;
    Produk* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

Produk* insertAVL(Produk* node, string k, string n, int h, int s) {
    if (!node) return new Produk{k, n, h, s, 1, NULL, NULL};

    if (k < node->kode)
        node->left = insertAVL(node->left, k, n, h, s);
    else if (k > node->kode)
        node->right = insertAVL(node->right, k, n, h, s);
    else {
        node->stok += s;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = height(node->left) - height(node->right);

    if (balance > 1 && k < node->left->kode)
        return rotasiKanan(node);
    if (balance < -1 && k > node->right->kode)
        return rotasiKiri(node);

    return node;
}

Produk* cariProduk(Produk* r, string k) {
    if (!r || r->kode == k) return r;
    return k < r->kode ? cariProduk(r->left, k) : cariProduk(r->right, k);
}

void tampilProdukPembeli(Produk* r) {
    if (!r) return;
    tampilProdukPembeli(r->left);
    cout << left << setw(20) << r->nama
         << setw(10) << r->harga
         << setw(6) << r->stok << endl;
    tampilProdukPembeli(r->right);
}

// =========================================================
// 3. STACK (RIWAYAT TRANSAKSI)
// =========================================================
struct Stack {
    string info;
    Stack* next;
};

Stack* topStack = NULL;

void push(string s) {
    topStack = new Stack{s, topStack};
}

void tampilRiwayat() {
    cout << "\n=== RIWAYAT TRANSAKSI ===\n";
    Stack* c = topStack;
    while (c) {
        cout << "- " << c->info << endl;
        c = c->next;
    }
}

// =========================================================
// 4. KERANJANG (DOUBLE LINKED LIST)
// =========================================================
struct Keranjang {
    string nama;
    int qty, subtotal;
    Keranjang *next, *prev;
};

Keranjang *headK = NULL, *tailK = NULL;

void tambahKeranjang(string n, int q, int s) {
    Keranjang* b = new Keranjang{n, q, s, NULL, NULL};
    if (!headK) headK = tailK = b;
    else {
        tailK->next = b;
        b->prev = tailK;
        tailK = b;
    }
}

// =========================================================
// 5. LOGIN PEMBELI
// =========================================================
KTP loginPembeli() {
    KTP p;
    cout << "\n===== LOGIN PEMBELI (KTP) =====\n";
    cout << "NIK   : "; cin >> p.nik;
    cin.ignore();
    cout << "Nama  : "; getline(cin, p.nama);
    cout << "Alamat: "; getline(cin, p.alamat);
    return p;
}

// =========================================================
// 6. TRANSAKSI + STRUK (SATU OUTPUT)
// =========================================================
int noTransaksi = 0;

void prosesCheckout(KTP p) {
    int total = 0;
    Keranjang* c = headK;

    cout << "\n========= STRUK BELANJA PC =========\n";
    cout << "Nama   : " << p.nama << endl;
    cout << "NIK    : " << p.nik << endl;
    cout << "Alamat : " << p.alamat << endl;
    cout << "-----------------------------------\n";

    while (c) {
        cout << c->nama << " x" << c->qty
             << " = Rp " << c->subtotal << endl;
        total += c->subtotal;
        c = c->next;
    }

    cout << "-----------------------------------\n";
    cout << "TOTAL BAYAR : Rp " << total << endl;

    noTransaksi++;

    // SIMPAN KE TXT
    ofstream f("Transaksi.txt", ios::app);
    f << "Pembelian Ke-" << noTransaksi << endl;
    f << "Nama : " << p.nama << endl;
    f << "NIK  : " << p.nik << endl;
    f << "Total: Rp " << total << endl;
    f << "---------------------------\n";
    f.close();

    // MASUK STACK (ADMIN)
    push("Pembelian Ke-" + to_string(noTransaksi) +
         " | Nama: " + p.nama +
         " | Total: Rp " + to_string(total));
}

// =========================================================
// 7. MENU ADMIN
// =========================================================
void menuAdmin() {
    int pil;
    do {
        cout << "\n===== MENU ADMIN =====\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Lihat Riwayat Transaksi\n";
        cout << "3. Logout\n";
        cout << "Pilih: ";
        cin >> pil;

        if (pil == 1) {
            string k, n;
            int h, s;
            cout << "Kode  : "; cin >> k;
            cin.ignore();
            cout << "Nama  : "; getline(cin, n);
            cout << "Harga : "; cin >> h;
            cout << "Stok  : "; cin >> s;
            rootProduk = insertAVL(rootProduk, k, n, h, s);
        }
        else if (pil == 2) {
            tampilRiwayat();
        }
    } while (pil != 3);
}

// =========================================================
// 8. MAIN
// =========================================================
int main() {
    int pilih;
    while (true) {
        cout << "\n===== SISTEM PENJUALAN PC =====\n";
        cout << "1. Admin\n2. Pembeli\n3. Keluar\nPilih: ";
        cin >> pilih;

        if (pilih == 1) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;
            if (u == "admin" && p == "123")
                menuAdmin();
        }
        else if (pilih == 2) {
            KTP pembeli = loginPembeli();

            cout << "\nNama Produk          Harga     Stok\n";
            cout << "-----------------------------------\n";
            tampilProdukPembeli(rootProduk);

            string k;
            int j;
            cout << "\nKode Produk: ";
            cin >> k;
            cout << "Jumlah: ";
            cin >> j;

            Produk* pr = cariProduk(rootProduk, k);
            if (pr && pr->stok >= j) {
                pr->stok -= j;
                tambahKeranjang(pr->nama, j, pr->harga * j);
                prosesCheckout(pembeli);
            }
        }
        else break;
    }
    return 0;
}

