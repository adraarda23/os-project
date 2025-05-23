#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FLOORS 10
#define NUM_APARTMENTS 4

// Ortak kaynakları korumak için mutex
pthread_mutex_t crane_mutex;

// Katlar arası sıralı ilerlemeyi sağlamak için semaphore
sem_t floor_semaphore[NUM_FLOORS];

// Daire inşaatını simüle eden fonksiyon
void* construct_apartment(void* arg) {
    int apartment_id = *(int*)arg;
    int floor = apartment_id / NUM_APARTMENTS;
    int apt = apartment_id % NUM_APARTMENTS + 1;
    
    printf("Kat %d, Daire %d inşaatı başladı.\n", floor + 1, apt);
    
    // Ortak kaynak (vinç) kullanımı
    pthread_mutex_lock(&crane_mutex);
    printf("Kat %d, Daire %d vinci kullanıyor.\n", floor + 1, apt);
    sleep(1); // Vinç kullanım süresi
    printf("Kat %d, Daire %d vinci bıraktı.\n", floor + 1, apt);
    pthread_mutex_unlock(&crane_mutex);
    
    // Daire inşaatı tamamlandı
    printf("Kat %d, Daire %d inşaatı tamamlandı.\n", floor + 1, apt);
    return NULL;
}

// Kat inşaatını simüle eden fonksiyon
void* construct_floor(void* arg) {
    int floor = *(int*)arg;
    
    // Önceki katın tamamlanmasını bekle (floor 0 hariç)
    if (floor > 0) {
        sem_wait(&floor_semaphore[floor - 1]);
    }
    
    printf("Kat %d inşaatı başladı.\n", floor + 1);
    
    // Her kat için daire thread'leri
    pthread_t apartment_threads[NUM_APARTMENTS];
    int apartment_ids[NUM_APARTMENTS];
    
    for (int i = 0; i < NUM_APARTMENTS; i++) {
        apartment_ids[i] = floor * NUM_APARTMENTS + i;
        pthread_create(&apartment_threads[i], NULL, construct_apartment, &apartment_ids[i]);
    }
    
    // Daire thread'lerinin tamamlanmasını bekle
    for (int i = 0; i < NUM_APARTMENTS; i++) {
        pthread_join(apartment_threads[i], NULL);
    }
    
    printf("Kat %d inşaatı tamamlandı.\n", floor + 1);
    
    // Bu katın tamamlandığını signal et (sonraki kat için)
    sem_post(&floor_semaphore[floor]);
    
    return NULL;
}

int main() {
    // Mutex ve semaphore'ları başlat
    pthread_mutex_init(&crane_mutex, NULL);
    
    // Tüm semaphore'ları 0 ile başlat
    for (int i = 0; i < NUM_FLOORS; i++) {
        sem_init(&floor_semaphore[i], 0, 0);
    }
    
    printf("Temel atıldı. İnşaat başlıyor...\n");
    
    // Her kat için thread oluştur
    pthread_t floor_threads[NUM_FLOORS];
    int floor_ids[NUM_FLOORS];
    
    for (int i = 0; i < NUM_FLOORS; i++) {
        floor_ids[i] = i;
        pthread_create(&floor_threads[i], NULL, construct_floor, &floor_ids[i]);
    }
    
    // Kat thread'lerinin tamamlanmasını bekle
    for (int i = 0; i < NUM_FLOORS; i++) {
        pthread_join(floor_threads[i], NULL);
    }
    
    printf("Apartman inşaatı tamamlandı!\n");
    
    // Mutex ve semaphore'ları temizle
    pthread_mutex_destroy(&crane_mutex);
    for (int i = 0; i < NUM_FLOORS; i++) {
        sem_destroy(&floor_semaphore[i]);
    }
    
    return 0;
}