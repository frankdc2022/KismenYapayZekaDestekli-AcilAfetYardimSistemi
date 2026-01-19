# 🚨 Disaster Management System (C++ Data Structures Project)

Bu proje, C++ kullanılarak geliştirilmiş, gelişmiş veri yapıları ve algoritmalar üzerine kurulu bir **Afet Yönetim Sistemi** simülasyonudur. Sistem, acil durum olaylarını önceliklendirerek yönetir, rota hesaplamaları yapar ve verileri JSON formatında saklar.

## 🛠 Kullanılan Veri Yapıları (Data Structures Implemented)

Proje içerisinde aşağıdaki veri yapıları, kullanım amaçlarına uygun olarak hibrit bir mimaride kullanılmıştır:

| Veri Yapısı | Kullanım Amacı | Kod İçindeki Karşılığı |
|:---:|:---|:---|
| **Priority Queue (Heap)** | Acil durumu yüksek olayların (Deprem, Yangın) en öne alınması ve ilk müdahale edilmesi için kullanılmıştır. | `priority_queue<Olay...>` |
| **Queue (FIFO)** | Aciliyeti düşük, normal prosedür gerektiren olayların sırayla işlenmesi için kullanılmıştır. | `queue<Olay> normalKuyruk` |
| **Stack (LIFO)** | İşlenen olayların geçmişini tutmak ve son yapılan müdahaleyi görüntülemek için kullanılmıştır. | `stack<Olay> gecmis` |
| **Hash Table (Map)** | Olay ID'sine göre O(1) karmaşıklığında hızlı arama yapmak için kullanılmıştır. | `unordered_map<int, Olay>` |
| **Hash Set** | Aynı olayın sisteme mükerrer (duplicate) girişini engellemek için kullanılmıştır. | `unordered_set<string> tekrarKontrol` |
| **Graph (Adjacency List)** | Şehirler arası ulaşım rotalarını ve bağlantılarını modellemek için kullanılmıştır. | `unordered_map<string, vector<string>>` |

## 🚀 Algoritmalar ve Özellikler

### 1. Akıllı Aciliyet Hesaplama (Smart Priority Algorithm)
Sistem, girilen olayın türüne (Deprem > Yangın > Sel), yaralı sayısına ve olayın gerçekleştiği saate (Gece/Gündüz) göre otomatik bir **Aciliyet Puanı (1-10)** atar.
* **Örnek:** Gece gerçekleşen ve çok yaralısı olan bir deprem, gündüz olan küçük bir selden daha yüksek puan alır ve `Priority Queue` sayesinde listenin en tepesine yerleşir.

### 2. Sıralama Algoritmaları (Sorting)
Olayların analizi için iki farklı sıralama algoritması implemente edilmiştir:
* **Quick Sort:** `std::sort` kullanılarak olaylar aciliyet sırasına göre O(N log N) karmaşıklığıyla sıralanır.
* **Bubble Sort:** Eğitim amaçlı karşılaştırma için manuel implemente edilmiştir.

### 3. Veri Kalıcılığı (JSON Persistence)
Proje, verilerin kaybolmaması için `nlohmann/json` kütüphanesini kullanır.
* Program kapatılırken tüm kuyruklar `veri.json` dosyasına serileştirilir (Serialization).
* Program açıldığında veriler tekrar uygun veri yapılarına (Heap/Queue) yüklenir (Deserialization).

## 💻 Kurulum ve Derleme (How to Build)

Projede harici `json.hpp` kütüphanesi kullanılmıştır. Derlemek için C++11 veya üzeri standart gereklidir.

```bash
# Gerekli dosyaları derle
g++ main.cpp -o disaster_system -std=c++11

# Çalıştır
./disaster_system
