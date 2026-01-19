#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// ---------------- OLAY YAPISI ----------------
struct Olay {
    int id;
    string sehir;
    string tur;
    int aciliyet;
    string zaman;

    void yaz() const {
        cout << "[" << id << "] "
            << sehir << " - " << tur
            << " | Aciliyet: " << aciliyet
            << " | Zaman: " << zaman << endl;
    }
};

// ---------------- PRIORITY QUEUE KIYAS ----------------
struct Kiyas {
    bool operator()(const Olay& a, const Olay& b) {
        return a.aciliyet < b.aciliyet;
    }
};

// ---------------- VERI YAPILARI ----------------
priority_queue<Olay, vector<Olay>, Kiyas> acilKuyruk;   // Heap
queue<Olay> normalKuyruk;                               // Queue
stack<Olay> gecmis;                                     // Stack
unordered_map<int, Olay> olayMap;                       // Hash Table
unordered_set<string> tekrarKontrol;                    // HashSet
unordered_map<string, vector<string>> rotaGraph;        // Graph

int sonID = 0;

// ---------------- GRAPH OLUŞTUR ----------------
void rotaOlustur() {
    rotaGraph["Ankara"] = { "Istanbul", "Konya" };
    rotaGraph["Istanbul"] = { "Ankara", "Bursa" };
    rotaGraph["Izmir"] = { "Manisa", "Aydin" };
}

// ---------------- TUM OLAYLARI GETIR ----------------
vector<Olay> tumOlaylariGetir() {
    vector<Olay> liste;

    auto tmpA = acilKuyruk;
    while (!tmpA.empty()) {
        liste.push_back(tmpA.top());
        tmpA.pop();
    }

    auto tmpN = normalKuyruk;
    while (!tmpN.empty()) {
        liste.push_back(tmpN.front());
        tmpN.pop();
    }

    return liste;
}

// ---------------- AKILLI ACILIYET ----------------
int akilliAciliyetHesapla(string tur, int yaraliSayisi, bool geceMi) {
    int puan = 0;

    if (tur == "deprem") puan += 5;
    else if (tur == "yangin") puan += 4;
    else if (tur == "sel") puan += 3;
    else puan += 2;

    if (yaraliSayisi >= 10) puan += 3;
    else if (yaraliSayisi >= 5) puan += 2;
    else if (yaraliSayisi > 0) puan += 1;

    if (geceMi) puan += 2;

    if (puan > 10) puan = 10;
    return puan;
}

// ---------------- OLAY EKLE ----------------
void olayEkle() {
    Olay o;
    o.id = ++sonID;

    cout << "Sehir: ";
    cin >> o.sehir;

    cout << "Olay turu (deprem/sel/yangin): ";
    cin >> o.tur;

    int yarali, geceSecim;
    cout << "Yarali sayisi: ";
    cin >> yarali;

    cout << "Gece mi? (1=Evet, 0=Hayir): ";
    cin >> geceSecim;
    bool geceMi = (geceSecim == 1);

    cout << "Zaman (GG/AA/YYYY SS:DD): ";
    cin.ignore();
    getline(cin, o.zaman);

    // HashSet → tekrar kontrol
    string anahtar = o.sehir + "-" + o.tur + "-" + o.zaman;
    if (tekrarKontrol.count(anahtar)) {
        cout << "\n Bu olay zaten kayitli (tekrar engellendi).\n";
        return;
    }
    tekrarKontrol.insert(anahtar);

    // Akıllı aciliyet
    o.aciliyet = akilliAciliyetHesapla(o.tur, yarali, geceMi);
    cout << "Sistem aciliyet puani: " << o.aciliyet << endl;

    if (o.aciliyet >= 5)
        acilKuyruk.push(o);
    else
        normalKuyruk.push(o);

    olayMap[o.id] = o;

    cout << "\n Olay eklendi.\n";
}

// ---------------- OLAY ISLE ----------------
void olayIsle() {
    if (!acilKuyruk.empty()) {
        Olay o = acilKuyruk.top();
        acilKuyruk.pop();
        gecmis.push(o);

        cout << "\n ACIL OLAY ISLENDI:\n";
        o.yaz();

        if (rotaGraph.count(o.sehir)) {
            cout << "Ulasim rotalari:\n";
            for (auto& r : rotaGraph[o.sehir])
                cout << " -> " << r << endl;
        }
    }
    else if (!normalKuyruk.empty()) {
        Olay o = normalKuyruk.front();
        normalKuyruk.pop();
        gecmis.push(o);

        cout << "\n NORMAL OLAY ISLENDI:\n";
        o.yaz();
    }
    else {
        cout << "\n Islenecek olay yok.\n";
    }
}

// ---------------- LISTELE ----------------
void listele() {
    cout << "\n--- ACIL OLAYLAR ---\n";
    auto tmpA = acilKuyruk;
    while (!tmpA.empty()) {
        tmpA.top().yaz();
        tmpA.pop();
    }

    cout << "\n--- NORMAL OLAYLAR ---\n";
    auto tmpN = normalKuyruk;
    while (!tmpN.empty()) {
        tmpN.front().yaz();
        tmpN.pop();
    }
}

// ---------------- GECMIS ----------------
void gecmisiGoster() {
    cout << "\n--- ISLENMIS OLAY GECMISI ---\n";
    auto tmp = gecmis;
    while (!tmp.empty()) {
        tmp.top().yaz();
        tmp.pop();
    }
}

// ---------------- OLAY ARA ----------------
void olayAra() {
    int id;
    cout << "Aranacak Olay ID: ";
    cin >> id;

    if (olayMap.count(id)) {
        olayMap[id].yaz();
    }
    else {
        cout << "Olay bulunamadi.\n";
    }
}

// ---------------- SIRALAMA ----------------
void bubbleSort() {
    vector<Olay> liste = tumOlaylariGetir();
    int n = liste.size();

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (liste[j].aciliyet < liste[j + 1].aciliyet)
                swap(liste[j], liste[j + 1]);

    cout << "\n--- BUBBLE SORT ---\n";
    for (auto& o : liste) o.yaz();
}

bool karsilastir(Olay a, Olay b) {
    return a.aciliyet > b.aciliyet;
}

void quickSort() {
    vector<Olay> liste = tumOlaylariGetir();
    sort(liste.begin(), liste.end(), karsilastir);

    cout << "\n--- QUICK SORT ---\n";
    for (auto& o : liste) o.yaz();
}

// ---------------- JSON ----------------
void kaydet() {
    json j;

    auto tmpA = acilKuyruk;
    while (!tmpA.empty()) {
        auto o = tmpA.top(); tmpA.pop();
        j["acil"].push_back({ {"id",o.id},{"sehir",o.sehir},{"tur",o.tur},{"aciliyet",o.aciliyet},{"zaman",o.zaman} });
    }

    auto tmpN = normalKuyruk;
    while (!tmpN.empty()) {
        auto o = tmpN.front(); tmpN.pop();
        j["normal"].push_back({ {"id",o.id},{"sehir",o.sehir},{"tur",o.tur},{"aciliyet",o.aciliyet},{"zaman",o.zaman} });
    }

    ofstream f("veri.json");
    f << j.dump(4);
    f.close();
}

void yukle() {
    ifstream dosya("veri.json");
    if (!dosya.is_open()) {
        cout << "\n Dosya bulunamadi.\n";
        return;
    }

    json j;
    dosya >> j;
    dosya.close();

    while (!acilKuyruk.empty()) acilKuyruk.pop();
    while (!normalKuyruk.empty()) normalKuyruk.pop();
    while (!gecmis.empty()) gecmis.pop();
    olayMap.clear();
    tekrarKontrol.clear();

    for (auto& o : j["acil"]) {
        Olay olay = { o["id"], o["sehir"], o["tur"], o["aciliyet"], o["zaman"] };
        acilKuyruk.push(olay);
        olayMap[olay.id] = olay;
        if (olay.id > sonID) sonID = olay.id;
    }

    for (auto& o : j["normal"]) {
        Olay olay = { o["id"], o["sehir"], o["tur"], o["aciliyet"], o["zaman"] };
        normalKuyruk.push(olay);
        olayMap[olay.id] = olay;
        if (olay.id > sonID) sonID = olay.id;
    }

    cout << "\n Veriler JSON dosyasindan yuklendi.\n";
}

// ---------------- MAIN ----------------
int main() {
    rotaOlustur();

    int secim;
    do {
        cout << "\n--- AFET YONETIM SISTEMI ---\n";
        cout << "1-Olay Ekle\n";
        cout << "2-Olay Isle\n";
        cout << "3-Listele\n";
        cout << "4-Gecmis\n";
        cout << "5-Ara\n";
        cout << "6-Kaydet (JSON)\n";
        cout << "7-Yukle (JSON)\n";
        cout << "8-Bubble Sort\n";
        cout << "9-Quick Sort\n";
        cout << "0-Cikis\n";

        cin >> secim;

        switch (secim) {
        case 1: olayEkle(); break;
        case 2: olayIsle(); break;
        case 3: listele(); break;
        case 4: gecmisiGoster(); break;
        case 5: olayAra(); break;
        case 6: kaydet(); break;
        case 7: yukle(); break;
        case 8: bubbleSort(); break;
        case 9: quickSort(); break;
        }
    } while (secim != 0);

    return 0;
}
