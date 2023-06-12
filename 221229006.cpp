#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dugum
{
    char *isim;
    int ogrno;
    int vize;
    int final;
    float ort;
    struct dugum *onceki;
    struct dugum *sonraki;
} dugum;

dugum *baslangic = NULL;

void ListeyeEkle(dugum **baslangic, int ogrno, char *isim, int vize, int final, float ort)
{
    dugum *yenidugum = (dugum *)malloc(sizeof(dugum));
    yenidugum->ogrno = ogrno;
    yenidugum->isim = strdup(isim);
    yenidugum->vize = vize;
    yenidugum->final = final;
    yenidugum->ort = ort;
    yenidugum->onceki = NULL;
    yenidugum->sonraki = NULL;

    if (*baslangic == NULL)
    {
        *baslangic = yenidugum;
        return;
    }

    dugum *aranan = *baslangic;
    if (aranan->ort < 60 && ort >= 60)
    {
        aranan->onceki = yenidugum;
        yenidugum->sonraki = aranan;
        *baslangic = yenidugum;
        return;
    }

    while (aranan != NULL)
    {
        if (aranan->sonraki == NULL)
        {
            aranan->sonraki = yenidugum;
            yenidugum->onceki = aranan;
            return;
        }
        else if (ort >= 60 && aranan->sonraki->ort < 60)
        {
            dugum *gecici = aranan->sonraki;
            aranan->sonraki = yenidugum;
            yenidugum->onceki = aranan;
            yenidugum->sonraki = gecici;
            gecici->onceki = yenidugum;
            return;
        }
        aranan = aranan->sonraki;
    }
}

void sil(dugum **baslangic, char *yeniad)
{
    dugum *aranan = *baslangic;
    while (aranan != NULL)
    {
        if (strcmp(aranan->isim, yeniad) == 0)
        {
            if (aranan->onceki == NULL) // basta
            {
                dugum *gecici = aranan->sonraki;
                gecici->onceki = NULL;
                *baslangic = gecici;
            }
            else if (aranan->sonraki == NULL) // sonda
            {
                dugum *gecici = aranan->onceki;
                gecici->sonraki = NULL;
            }
            else
            {
                dugum *gecici1 = aranan->onceki;
                dugum *gecici2 = aranan->sonraki;
                gecici1->sonraki = gecici2;
                gecici2->onceki = gecici1;
            }
            free(aranan);
            return;
        }
        aranan = aranan->sonraki;
    }
}

void Guncelle(dugum **baslangic, char *yeniad)
{
    dugum *aranan = *baslangic;
    int vize, final;
    float ort;
    while (aranan != NULL)
    {
        if (strcmp(aranan->isim, yeniad) == 0)
        {
            sil(baslangic, yeniad);
            printf("%s kisisinin yeni vize notu : ", yeniad);
            scanf("%d", &vize);
            printf("%s kisisinin yeni final notu : ", yeniad);
            scanf("%d", &final);
            ort = (vize * 0.4) + (final * 0.6);
            ListeyeEkle(baslangic, aranan->ogrno, yeniad, vize, final, ort);
            return;
        }
        aranan = aranan->sonraki;
    }
    printf("\nBoyle bir isim listede bulunmamaktadir..");
}

void BastanSilme(dugum **baslangic)
{
    dugum *aranan = *baslangic;
    while (aranan != NULL)
    {
        if (aranan->ogrno % 2 == 0 && aranan->ort < 60)
        {
            if (aranan->onceki == NULL) // basta
            {
                dugum *gecici = aranan->sonraki;
                gecici->onceki = NULL;
                *baslangic = gecici;
            }
            else if (aranan->sonraki == NULL) // sonda
            {
                dugum *gecici = aranan->onceki;
                gecici->sonraki = NULL;
            }
            else // ortada
            {
                dugum *gecici1 = aranan->onceki;
                dugum *gecici2 = aranan->sonraki;
                gecici1->sonraki = gecici2;
                gecici2->onceki = gecici1;
            }
            free(aranan);
            return;
        }
        aranan = aranan->sonraki;
    }
}

void SondanSilme(dugum **baslangic)
{
    dugum *aranan = *baslangic;
    while (aranan != NULL)
    {
        aranan = aranan->sonraki;
    }
    // aranan=aranan->onceki;
    while (aranan != NULL)
    {
        if (aranan->ogrno % 2 == 0 && aranan->ort < 60) // ogrenci nosu cift ve dersten kalan
        {
            if (aranan->sonraki == NULL) // en sondaysa
            {
                dugum *gecici = aranan->onceki;
                gecici->sonraki = NULL;
            }
            else if (aranan->onceki == NULL) // en bastaysa
            {
                dugum *gecici = aranan->sonraki;
                gecici->onceki = NULL;
                *baslangic = gecici;
            }
            else // ortalardaysa
            {
                dugum *gecici1 = aranan->onceki;
                dugum *gecici2 = aranan->sonraki;
                gecici1->sonraki = gecici2;
                gecici2->onceki = gecici1;
            }
            free(aranan);
            return;
        }
        aranan = aranan->onceki;
    }
}

void Listele(dugum *baslangic)
{
    printf("\n\n");
    dugum *ilkdugum = baslangic;
    while (ilkdugum != NULL)
    {
        printf("%d %s %d %d\n", ilkdugum->ogrno, ilkdugum->isim, ilkdugum->vize, ilkdugum->final);
        ilkdugum = ilkdugum->sonraki;
    }
}

int main()
{
    FILE *dosya = fopen("ogrenci.txt", "r");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    int tercih = 1;
    int ogrno, vize, final;
    char isim[50];
    float ort;

    while (fscanf(dosya, "%d %s %d %d", &ogrno, isim, &vize, &final) != EOF)
    {
        ort = (vize * 0.4) + (final * 0.6);
        ListeyeEkle(&baslangic, ogrno, isim, vize, final, ort);
    }

    do
    {
        do
        {
            printf("\n0-Cikis\n1-Liste Olustur\n2-Listele\n3-Guncelle\n4-Sil\nBir tercih yapiniz: ");
            scanf("%d", &tercih);
        } while (tercih != 0 && tercih != 1 && tercih != 2 && tercih != 3 && tercih != 4);

        if (tercih == 1)
        {
            while (fscanf(dosya, "%d %s %d %d", &ogrno, isim, &vize, &final) != EOF)
            {
                ort = (vize * 0.4) + (final * 0.6);
                ListeyeEkle(&baslangic, ogrno, isim, vize, final, ort);
            }
        }
        else if (tercih == 2)
        {
            Listele(baslangic);
        }
        else if (tercih == 3)
        {
            char yeniad[50];
            printf("Guncelleme yapmak istediginiz ismi giriniz: ");
            scanf("%s", &yeniad);

            Guncelle(&baslangic, yeniad);
        }
        else if (tercih == 4)
        {
        don:
            char secim[2];
            printf("Silme islemi seciniz (b -> bastan silme, s -> sondan silme): ");
            scanf("%s", &secim);

            if (strcmp(secim, "b") == 0) // bastan silme
            {
                BastanSilme(&baslangic);
            }
            else if (strcmp(secim, "s") == 0) // sondan silme
            {
                SondanSilme(&baslangic);
            }
            else // yanlıs secim durumunda
            {
                printf("Yanlis ifade girildi..");
                goto don;
            }
        }
        else
        {
            break;
        }

    } while (tercih != 0);

    printf("\nCikis yapiliyor..");
    printf("\nCikis yapildi.");
}