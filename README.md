# Apartment Thread Mutex Program

Bu proje, çoklu iş parçacığı (thread) kullanarak bir apartman yönetim senaryosunu simüle eden basit bir C programıdır. Program, paylaşılan verilere eşzamanlı erişimi kontrol etmek için `pthread` kütüphanesinden `mutex` (karşılıklı dışlama) mekanizmasını kullanır.

## Amaç

Bu programın amacı, aynı anda birden fazla iş parçacığının eriştiği paylaşılan kaynaklarda veri tutarsızlıklarını önlemektir. Bu örnek, özellikle `pthread_create`, `pthread_join` ve `pthread_mutex_lock/unlock` fonksiyonlarının nasıl kullanılacağını göstermektedir.

## Derleme ve Çalıştırma

Aşağıdaki komutlarla programı derleyebilir ve çalıştırabilirsiniz:

```bash
gcc -o apartment apartment.c -pthread
./apartment
