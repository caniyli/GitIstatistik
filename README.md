# GitIstatistik

## 1. Git İstatistik Nedir?

Git İstatistik programı Git hesabınızdan attığınız commitleri görsel olarak görüp, yaptığınız çalışmaları analiz etmeninizi sağlayan bir Qt projesidir. Toplam commit sayınız, hafta içi, haftasonu attığınız commit sayısı
gibi bir çok veriyi gerek görsel gerek sayısal olarak analiz edebilirsiniz. Gerekli bilgileri verdikten sonra istatistiklerinizi seçtiğiniz yıla,  aya, tarih aralığına göre farklı seçeneklerde
çeşitli grafikler ile görsel bir şekilde karşınızda görebilirsiniz. Eğer isterseniz arkadaşlarınızın commit bilgileriyle kendinizinkini karşılaştırabilirsiniz.

## 2. Nasıl İndiririm?

### 2.1 Projeyi İndirme
Git İstatistik'i kullanabilmek için öncelikle bilgisayarınıza git üzerinden klonlamalısınız.

```
git clone "https://github.com/caniyli/GitIstatistik.git"
```

### 2.2 Projeyi Derleme

Git İstatistik Projesi Qt ile hazırlanmıştır
```
mkdir Build
cd Build
qmake ../DonutChart.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
make -j8
```

Satırını ekledikten sonra projeyi kullanabilirsiniz

## 3. Nasıl Kullanırım?

Projeyi kullanabilmek için girdi değerlerini bir .txt dosyasında tutmanız gerekmektedir. Bu değerlere istenilen formatta erişebilmek için Shell'de aşağıdaki komutu kullabilirsiniz

```
cd $PROJECT_PATH;  git --no-pager log --author="$USERNAME" --date=raw --pretty=format:"%h%x09%an%x09%ad%x09%s" 2>&1 > $TARGET/"$USERNAME".txt
```

![Image of Menu](https://github.com/caniyli/GitIstatistik/blob/main/images/Menu.png)

Yukarıda görünen menünün en üstünde combo box ile okunacak dosyanın yolunu ve adını seçiyoruz. Bu işlem sonunda istatistiklerimizi elde etmiş oluyoruz ve bilgiler karşımıza geliyor.

Daha sonra menü üzerinden yaptığımız seçimlerle karşımıza grafikler ve görsel analiz araçları geliyor. Örneğin yıllık bir pasta grafiği:

![Image of PieChart](https://github.com/caniyli/GitIstatistik/blob/main/images/YearlyChart.png)

Eğer 2 Tarih arası verileri göster seçeneğini işaretlersek karşımıza bir takvim ekranı çıkıyor ve gün gün atılan commit bilgilerini inceleyebilirsiniz.

![Image of Calendar](https://github.com/caniyli/GitIstatistik/blob/main/images/Calendar.png)
