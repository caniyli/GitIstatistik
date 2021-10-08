# GitIstatistik

## 1. Git İstatistik Nedir?

Git İstatistik programı Git hesabınızdan attığınız commitleri görsel olarak görüp, yaptığınız çalışmaları analiz etmenizi sağlar. Toplam commit sayınız, hafta içi, haftasonu attığınız commit sayısı
gibi bir çok veriyi gerek görsel gerek sayısal olarak analiz edebilirsiniz. Gerekli bilgileri verdikten sonra istatistiklerinizi seçtiğiniz yıla,  aya, tarih aralığına göre farklı seçeneklerde
çeşitli grafikler ile görsel bir şekilde karşınızda görebilirsiniz. Eğer isterseniz arkadaşlarınızın commit bilgileriyle kendinizinkini karşılaştırabilirsiniz.

## 2. Nasıl İndiririm?

Git İstatistik'i kullanabilmek için öncelikle bilgisayarınıza git üzerinden klonlamalısınız.

```
git clone "https://github.com/caniyli/GitIstatistik.git"
```
Daha sonra Qt ile açılan standart bir QWidget projesinin .pro uzantılı dosyasına source ve header dosyalarını ekleyip, son olarak QChart kütüphanesini kullanabilmek için 
.pro uzantılı dosyanın en üstüne;

```
QT += charts
```

Satırını ekledikten sonra projeyi kullanabilirsiniz

## 3. Nasıl Kullanırım?

![Image of Menu](https://github.com/caniyli/GitIstatistik/blob/main/images/Menu.png)

Yukarıda görünen menünün en üstünde combo box ile okunacak dosyanın yolunu ve adını seçiyoruz. Bu işlem sonunda istatisklerimizi elde etmiş oluyoruz ve bilgiler karşımıza geliyor.

Daha sonra menü üzerinden yaptığımız seçimlerle karşımıza grafikler ve görsel analiz araçları geliyor. Örneğin yıllık bir pasta grafiği:

![Image of PieChart](https://github.com/caniyli/GitIstatistik/blob/main/images/YearlyChart.png)

Eğer 2 Tarih arası verileri göster seçeneğini işaretlersek karşımıza bir takvim ekranı çıkıyor ve gün gün atılan commit bilgileriniinceleyebiliyoruz.

![Image of Calendar](https://github.com/caniyli/GitIstatistik/blob/main/images/Calendar.png)
