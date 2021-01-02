#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

#define TYPE_OF_OGRENCI 0
#define TYPE_OF_OGRETMEN 1
#define TYPE_OF_DERS 2
#define TYPE_OF_DERSKAYIT 3
#define TYPE_OF_DERSOGRETMENKONTROL 4
#define TYPE_OF_AYARLAR 5

#define FILE_EXIST 1
#define FILE_NONE 0
#define FILE_EMPTY -1

#define PATH_SIZE 260
/*This value(260) is Maximum Path Length Limitation of Windows 10*/


//STRUCTLAR
typedef struct Ogrenci{
	int id;
	char adi[20];
	char soyadi[20];
	int toplam_ders;
	int toplam_kredi;
	struct Ogrenci *next;
}OGRENCI;

typedef struct Ogretmen{
	int id;
	char adi[20];
	char soyadi[20];
	char unvan[20];
	struct Ogretmen *next;
}OGRETMEN;

typedef struct Ders{
	char id[10];
	char adi[20];
	int kredi;
	int kontenjan;
	int ogretmen_id;
	struct Ders *next;
}DERS;

typedef struct OgrenciDersKayit{
	int id;//AUTO INCREMENT
	int ogrenci_id;
	char ders_id[10];
	int kayit_durumu;
	char kayit_tarihi[10];
	struct OgrenciDersKayit *next;
}OGRENCIDERSKAYIT;

typedef struct Ayarlar{
	int dersSinir;
	int krediSinir;
	int dersKayitAutoINC;
	int dersKontenjan;
}AYARLAR;
/////////////

///Summary - PROTOTIPLER
void OPENFILE(FILE **, char *, char *);
void FILETOSTRUCT(void **, int, char *);
void SHOWSTRUCT(void *, int);
void LOADSTRUCT(OGRENCI **, OGRETMEN **, DERS **, OGRENCIDERSKAYIT **, char *, char *, char *, char *);
int FILEISEXIST(char *);
int IS_ID_EXIST(void **, int, char *, int);
void STRUCTTOFILE(void **, int, char *);
void DELETEBYID(void **, int, char *, int, char *);//bu ikisi i�in fonksiyon pointer� kullanarak yap
void UPDATEBYID(void **, void**, int, char *, int, char *);
/////////////

int main(){
	setlocale(LC_ALL, "Turkish");
	
	FILE *dosya;
	OGRETMEN *ogretmenHead = NULL, *ogretmenTemp;
	OGRENCI *ogrenciHead = NULL, *ogrenciTemp;
	DERS *dersHead = NULL, *dersTemp;
	OGRENCIDERSKAYIT *dersKayitHead, *dersKayitTemp, *dersKayitTemp2;
	AYARLAR *ayarlar = (AYARLAR *) malloc(sizeof(AYARLAR));
	int secim = 0, tempNumber, kayitEr, ogrenciEr, dersEr;
	void *tempVoid, *tempVoid2;
	char *OGRENCIYOL, *OGRETMENYOL, *DERSYOL, *DERSKAYITYOL, *AYARLARYOL, tempChar[10];
	
	OGRENCIYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	OGRETMENYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	DERSYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	DERSKAYITYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	AYARLARYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	
	/*printf("***�N AYAR SAYFASINA HO�GELD�N�Z***\n\n");
	printf("��renci Dosyas� Yolunu Giriniz (OGRENCILER.txt): "); scanf("%s", OGRENCIYOL);
	printf("��retmen Dosyas� Yolunu Giriniz (OGRETMENLER.txt): "); scanf("%s", OGRETMENYOL);
	printf("Ders Dosyas� Yolunu Giriniz (DERSLER.txt): "); scanf("%s", DERSYOL);
	printf("Ders-Kay�t Dosyas� Yolunu Giriniz (OGRENCIDERSKAYIT.txt): "); scanf("%s", DERSKAYITYOL);
	printf("Ayarlar Dosyas� Yolunu Giriniz (AYARLAR.txt): "); scanf("%s", AYARLARYOL);*/
	OGRENCIYOL = "OGRENCILER.txt";
	OGRETMENYOL = "OGRETMENLER.txt";
	DERSYOL = "DERSLER.txt";
	DERSKAYITYOL = "OGRENCIDERSKAYIT.txt";
	AYARLARYOL = "AYARLAR.txt";
	
	LOADSTRUCT(&ogrenciHead, &ogretmenHead, &dersHead, &dersKayitHead, OGRENCIYOL, OGRETMENYOL, DERSYOL, DERSKAYITYOL);
	
	if(ISFILEEXIST(AYARLARYOL) != FILE_EXIST){//AYAR DOSYASI KONTROL�
		if(ISFILEEXIST(AYARLARYOL) == FILE_NONE)//Dosya var de�ilse ve dosya yoktuysa olu�tur
			if((dosya = fopen(AYARLARYOL, "w")) == NULL){
				printf("Ayarlar dosyas� ol�turulurken hata meydana geldi! ��k�� yap�l�yor..");
				exit(1);
			}else fclose(dosya);
			
		OPENFILE(&dosya, AYARLARYOL, "w");
		printf("G�r�n��e bak�l�rsa Ayarlar dosyan olu�turulmam�� veya i�erisi bo�.\nHadi beraber olu�tural�m.\n1-Ders S�n�r�: ");
		scanf("%d", &ayarlar->dersSinir);
		printf("2-Kredi S�n�r: ");
		scanf("%d", &ayarlar->krediSinir);
		printf("3-Ders Kontenjan S�n�r: ");
		scanf("%d", &ayarlar->dersKontenjan);
		ayarlar->dersKayitAutoINC = 0;
		
		fprintf(dosya, "%d %d %d %d", ayarlar->dersSinir, ayarlar->krediSinir, ayarlar->dersKayitAutoINC, ayarlar->dersKontenjan);
		fclose(dosya);
	}else{//Dosya varsa
		OPENFILE(&dosya, AYARLARYOL, "r");
		fscanf(dosya, "%d %d %d %d", &ayarlar->dersSinir, &ayarlar->krediSinir, &ayarlar->dersKayitAutoINC, &ayarlar->dersKontenjan);
		fclose(dosya);
	}
	
	while(secim != -1){
		system("cls");
		printf("***WELCOME TO MY HOMEWORK***\n\n");
		
		printf("---��retim �yesi ��lemleri---\n1-Ekle\n2-Sil\n3-G�ncelle\n---\n");
		printf("---Ders ��lemleri---\n4-Ekle\n5-Sil\n6-G�ncelle\n---\n");
		printf("---��renci ��lemleri---\n7-Ekle\n8-Sil\n9-G�ncelle\n---\n");
		printf("---��renci-Ders Kay�t ��lemleri---\n10-Ekle\n11-G�ncelle\n---\n");
		printf("Seciminiz (��kmak i�im -1): ");
		scanf("%d", &secim);
		
		if(secim == 1){//��retmen Ekle
			system("cls");
			printf("***��RETMEN EKLEME SAYFASINA HO�GELD�N�Z***\n\n");
			ogretmenTemp = (OGRETMEN *) malloc(sizeof(OGRETMEN));
			tempVoid = &ogretmenHead;//uyar� almamak i�in b�yle bir aktarma kulland�m.
			do{
				printf("��retmen ID: ");
				scanf("%d", &ogretmenTemp->id);
				if(IS_ID_EXIST(tempVoid, ogretmenTemp->id, NULL, TYPE_OF_OGRETMEN) || ogretmenTemp->id < 0)
					printf("Var olan bir ID veya negatif bir say� girdiniz. Tekrar deneyiniz!\n");
			}while(IS_ID_EXIST(tempVoid, ogretmenTemp->id, NULL, TYPE_OF_OGRETMEN) || ogretmenTemp->id < 0);
			printf("��retmen Ad�: ");
			scanf("%s", &ogretmenTemp->adi);
			printf("��retmen Soyad�: ");
			scanf("%s", &ogretmenTemp->soyadi);
			printf("��retmen Unvan: ");
			scanf("%s", &ogretmenTemp->unvan);
			ogretmenTemp->next = ogretmenHead;
			ogretmenHead = ogretmenTemp;
			STRUCTTOFILE(tempVoid, TYPE_OF_OGRETMEN, OGRETMENYOL);
			printf("Ders kayd� ba�ar�l�!\nDevam etmek i�in bir tu�a bas�n...");
			getch();
		}
		else if(secim == 2){//��retmen Sil
			system("cls");
			printf("***��RETMEN S�LME SAYFASINA HO�GELD�N�Z***\n\n");
			SHOWSTRUCT(ogretmenHead, TYPE_OF_OGRETMEN);
			tempVoid = &ogretmenHead;
			do{
				printf("\n��retmen ID (��k�� i�in -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRETMEN) && tempNumber != -1)
					printf("Sistemde kay�tl� olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRETMEN) && tempNumber != -1);

			tempVoid = &dersHead;
			if(tempNumber != -1)
				if(IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_DERSOGRETMENKONTROL))
					printf("Ders veren bir ��retmen silmek istiyorsunuz. Bunun i�in �nce dersi silmelisiniz!");
				else{
					tempVoid = &ogretmenHead;
					DELETEBYID(tempVoid, tempNumber, NULL, TYPE_OF_OGRETMEN, OGRETMENYOL);
					printf("Silme i�lemi ba�ar�l�!");
				}
			printf("\nDevam etmek i�in bir tu�a bas�n�z...");
			getch();
		}
		else if(secim == 3){//��retmen G�ncelle
			system("cls");
			printf("***��RETMEN G�NCELLEME SAYFASINA HO�GELD�N�Z***\n\n");
			SHOWSTRUCT(ogretmenHead, TYPE_OF_OGRETMEN);
			tempVoid = &ogretmenHead;
			do{
				printf("\n��retmen ID (��k�� i�in -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRETMEN) && tempNumber != -1)
					printf("Sistemde kay�tl� olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRETMEN) && tempNumber != -1);
			
			if(tempNumber != -1){
				UPDATEBYID(tempVoid, NULL, tempNumber, NULL, TYPE_OF_OGRETMEN, OGRETMENYOL);
				printf("G�ncelleme i�lemi ba�ar�l�!");
			}
			printf("\nDevam etmek i�in bir tu�a bas�n�z...");
			getch();
		}
		else if(secim == 4){//Ders Ekleme
			system("cls");
			printf("***DERS EKLEME SAYFASINA HO�GELD�N�Z***\n\n");
			dersTemp = (DERS *) malloc(sizeof(DERS));
			tempVoid = &dersHead;//uyar� almamak i�in b�yle bir aktarma kulland�m.
			do{
				printf("Ders ID: ");
				scanf("%s", &dersTemp->id);
				if(IS_ID_EXIST(tempVoid, 0, dersTemp->id, TYPE_OF_DERS))
					printf("Var olan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(IS_ID_EXIST(tempVoid, 0, dersTemp->id, TYPE_OF_DERS));
			printf("Ders Ad�: ");
			scanf("%s", &dersTemp->adi);
			printf("Ders Kredisi: ");
			scanf("%d", &dersTemp->kredi);
			dersTemp->kontenjan = 0; //Varsay�lan de�er
			tempVoid = &ogretmenHead;
			do{
				printf("��retmen ID (yoksa -1): ");
				scanf("%d", &dersTemp->ogretmen_id);
			}while(!IS_ID_EXIST(tempVoid, dersTemp->ogretmen_id, NULL, TYPE_OF_OGRETMEN) && dersTemp->ogretmen_id != -1);
			
			dersTemp->next = dersHead;
			dersHead = dersTemp;
			tempVoid = &dersHead;
			STRUCTTOFILE(tempVoid, TYPE_OF_DERS, DERSYOL);
			printf("Ders kayd� ba�ar�l�!\nDevam etmek i�in bir tu�a bas�n...");
			getch();
		}
		else if(secim == 5){//Ders Silme ---- ��RENCI DERS KAYITLARI YAPILDIKTAN SONRA YAPILACAK
			
		}
		else if(secim == 6){//Ders G�ncelleme
			system("cls");
			printf("***DERS G�NCELLEME SAYFASINA HO�GELD�N�Z***\n\n");
			SHOWSTRUCT(dersHead, TYPE_OF_DERS);
			tempVoid = &dersHead;
			tempVoid2 = &ogretmenHead;
			do{
				printf("\nDers ID (��k�� i�in -1): ");
				scanf("%s", tempChar);
				if(!IS_ID_EXIST(tempVoid, 0, tempChar, TYPE_OF_DERS) && strcmp(tempChar, "-1"))
					printf("Sistemde kay�tl� olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, 0, tempChar, TYPE_OF_DERS) && strcmp(tempChar, "-1"));
			
			if(strcmp(tempChar, "-1")){
				UPDATEBYID(tempVoid, tempVoid2, 0, tempChar, TYPE_OF_DERS, DERSYOL);
				printf("G�ncelleme i�lemi ba�ar�l�!");
			}
			
			printf("\nDevam etmek i�in bir tu�a bas�n�z...");
			getch();
		}
		else if(secim == 7){//��renci Ekleme
			system("cls");
			printf("***��RENC� EKLEME SAYFASINA HO�GELD�N�Z***\n\n");
			ogrenciTemp = (OGRENCI *) malloc(sizeof(OGRENCI));
			tempVoid = &ogrenciHead;//uyar� almamak i�in b�yle bir aktarma kulland�m.
			do{
				printf("��renci ID: ");
				scanf("%d", &ogrenciTemp->id);
				if(IS_ID_EXIST(tempVoid, ogrenciTemp->id, NULL, TYPE_OF_OGRENCI) || ogrenciTemp->id < 0)
					printf("Var olan bir ID veya negatif bir say� girdiniz. Tekrar deneyiniz!\n");
			}while(IS_ID_EXIST(tempVoid, ogrenciTemp->id, NULL, TYPE_OF_OGRENCI) || ogrenciTemp->id < 0);
			printf("��renci Ad�: ");
			scanf("%s", &ogrenciTemp->adi);
			printf("��renci Soyad�: ");
			scanf("%s", &ogrenciTemp->soyadi);
			ogrenciTemp->toplam_ders = ogrenciTemp->toplam_kredi = 0;//Varsay�lan olarak 0
			ogrenciTemp->next = ogrenciHead;
			ogrenciHead = ogrenciTemp;
			STRUCTTOFILE(tempVoid, TYPE_OF_OGRENCI, OGRENCIYOL);
			printf("��renci kayd� ba�ar�l�!\nDevam etmek i�in bir tu�a bas�n...");
			getch();
		}
		else if(secim == 8){//��renci Sil
			system("cls");
			printf("***��RENC� S�LME SAYFASINA HO�GELD�N�Z***\n\n");
			SHOWSTRUCT(ogrenciHead, TYPE_OF_OGRENCI);
			tempVoid = &ogrenciHead;
			do{
				printf("\n��renci ID (��k�� i�in -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRENCI) && tempNumber != -1)
					printf("Sistemde kay�tl� olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRENCI) && tempNumber != -1);
			
			if(tempNumber != -1){
				DELETEBYID(tempVoid, tempNumber, NULL, TYPE_OF_OGRENCI, OGRENCIYOL);
				printf("Silme i�lemi ba�ar�l�!");
			}
			printf("\nDevam etmek i�in bir tu�a bas�n�z...");
			getch();
		}
		else if(secim == 9){//��renci G�ncelle
			system("cls");
			printf("***��RENC� G�NCELLEME SAYFASINA HO�GELD�N�Z***\n\n");
			SHOWSTRUCT(ogrenciHead, TYPE_OF_OGRENCI);
			tempVoid = &ogrenciHead;
			do{
				printf("\n��renci ID (��k�� i�in -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRENCI) && tempNumber != -1)
					printf("Sistemde kay�tl� olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, NULL, TYPE_OF_OGRENCI) && tempNumber != -1);
			
			if(tempNumber != -1){
				UPDATEBYID(tempVoid, NULL, tempNumber, NULL, TYPE_OF_OGRENCI, OGRENCIYOL);
				printf("G�ncelleme i�lemi ba�ar�l�!");
			}
			printf("\nDevam etmek i�in bir tu�a bas�n�z...");
			getch();
		}
		else if(secim == 10){
			system("cls");
			printf("***��RENC�-DERS KAYIT SAYFASINA HO�GELD�N�Z***\n\n");
			dersKayitTemp = (OGRENCIDERSKAYIT *) malloc(sizeof(OGRENCIDERSKAYIT));
			do{
				tempVoid = &ogrenciHead;//uyar� almamak i�in b�yle bir aktarma kulland�m.
				do{
					printf("\n��renci ID: ");
					scanf("%d", &dersKayitTemp->ogrenci_id);
					if(!IS_ID_EXIST(tempVoid, dersKayitTemp->ogrenci_id, NULL, TYPE_OF_OGRENCI) || dersKayitTemp->ogrenci_id < 0)
						printf("Olmayan bir ID veya negatif bir say� girdiniz. Tekrar deneyiniz!\n");
				}while(!IS_ID_EXIST(tempVoid, dersKayitTemp->ogrenci_id, NULL, TYPE_OF_OGRENCI) || dersKayitTemp->ogrenci_id < 0);
				ogrenciTemp = ogrenciHead;
				while(ogrenciTemp->id != dersKayitTemp->ogrenci_id)
					ogrenciTemp = ogrenciTemp->next;
				
				tempVoid = &dersHead;
				do{
					printf("Ders ID: ");
					scanf("%s", &dersKayitTemp->ders_id);
					if(!IS_ID_EXIST(tempVoid, 0, dersKayitTemp->ders_id, TYPE_OF_DERS))
						printf("Olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
				}while(!IS_ID_EXIST(tempVoid, 0, dersKayitTemp->ders_id, TYPE_OF_DERS));
				dersTemp = dersHead;
				while(strcmp(dersTemp->id, dersKayitTemp->ders_id))
					dersTemp = dersTemp->next;
					
				printf("Kay�t Tarihi (gg-aa-yyyy): ");
				scanf("%s", &dersKayitTemp->kayit_tarihi);
				
				//Ayn� ��renci-ders kayd�n�n olup olmad���n� kontrol ediyor
				dersKayitTemp2 = dersKayitHead;
				/*while((dersKayitTemp2->ogrenci_id != dersKayitTemp->ogrenci_id && strcmp(dersKayitTemp2->ders_id, dersKayitTemp->ders_id) && strcmp(dersKayitTemp2->kayit_tarihi, dersKayitTemp->kayit_tarihi)) && dersKayitTemp2 != NULL)
					dersKayitTemp2 = dersKayitTemp2->next;
				if(dersKayitTemp2 != NULL) kayitEr = 0;
				else kayitEr = 1;*/
				tempNumber = 1;
				while(dersKayitTemp2 != NULL && tempNumber){
					if(dersKayitTemp2->ogrenci_id == dersKayitTemp->ogrenci_id)
						if(!strcmp(dersKayitTemp2->ders_id, dersKayitTemp->ders_id))
							if(!strcmp(dersKayitTemp2->kayit_tarihi, dersKayitTemp->kayit_tarihi))
								tempNumber = 0;
					
					if(tempNumber)
						dersKayitTemp2 = dersKayitTemp2->next;
				}
				kayitEr = tempNumber;
				
				//Dersin kontenjan�n�n a��l�p a��lmad���n� kontrol ediyor
				while(strcmp(dersTemp->id, dersKayitTemp->ders_id))
					dersTemp = dersTemp->next;
				if(dersTemp->kontenjan + 1 <= ayarlar->dersKontenjan) dersEr = 1;
				else dersEr = 0;
				
				//��rencinin ders s�n�r�n� veya kredi s�n�r�n� a��p a�mad��� kontrol ediliyor
				while(ogrenciTemp->id != dersKayitTemp->ogrenci_id)
					ogrenciTemp = ogrenciTemp->next;
				if(ogrenciTemp->toplam_ders + 1 < ayarlar->dersSinir && ogrenciTemp->toplam_kredi + dersTemp->kredi < ayarlar->krediSinir) ogrenciEr = 1;
				else ogrenciEr = 0;
				
				if(!(kayitEr * dersEr * ogrenciEr)){
					printf("Bu hatay� alma sebepleriniz;\nVar olan bir kay�t girmi� olabilirsiniz.\n* Dersin kontenjan�n� a�m�� olabilirsiniz.\n* ��rencinin ders s�n�r� veya kredi s�n�r�n� a�m�� olabilirsiniz.\nDe�i�iklik yapmak i�in G�ncelleme yapabilirsiniz\n��k�� i�in -1, Tekrar denemek i�in 0!: ");
					scanf("%d", &tempNumber);
				}
					
			}while(!(kayitEr * dersEr * ogrenciEr) && tempNumber != -1);

			if(tempNumber != -1){
				//dersKayit ID si artt�r�ld�
				ayarlar->dersKayitAutoINC++;
				dersKayitTemp->id = ayarlar->dersKayitAutoINC;
				
				dersKayitTemp->kayit_durumu = 1;//Varsay�lan olarak kay�tl�
				dersKayitTemp->next = dersKayitHead;
				dersKayitHead = dersKayitTemp;
				
				dersTemp->kontenjan++;
				ogrenciTemp->toplam_ders++;
				ogrenciTemp->toplam_kredi += dersTemp->kredi;
				
				
				tempVoid = &dersHead;
				STRUCTTOFILE(tempVoid, TYPE_OF_DERS, DERSYOL);
				tempVoid = &ogrenciHead;
				STRUCTTOFILE(tempVoid, TYPE_OF_OGRENCI, OGRENCIYOL);
				tempVoid = &dersKayitHead;
				STRUCTTOFILE(tempVoid, TYPE_OF_DERSKAYIT, DERSKAYITYOL);
				tempVoid = &ayarlar;
				STRUCTTOFILE(tempVoid, TYPE_OF_AYARLAR, AYARLARYOL);
				printf("��renci-Ders kayd� ba�ar�l�!");
			}else{
				free(dersKayitTemp);
			}
			printf("\nDevam etmek i�in bir tu�a bas�n...");
			getch();
		}
	}
	return 0;
}

void UPDATEBYID(void **headP, void** headP2, int ID, char *IDC, int choise, char *path){
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	OGRENCIDERSKAYIT *tempKayit;
	
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = *headP;
		while(tempOgrenci->id != ID)
			tempOgrenci = tempOgrenci->next;
		printf("----\n��renci ID: %d\n��renci Ad�: %s\n��renci Soyad�: %s\n��renci Toplam Ders: %d\n��renci Toplam Kredi: %d\n----\n", ID, tempOgrenci->adi, tempOgrenci->soyadi, tempOgrenci->toplam_ders, tempOgrenci->toplam_kredi);
		printf("��renci Ad�: "); scanf("%s", tempOgrenci->adi);
		printf("��renci Soyad�: "); scanf("%s", tempOgrenci->soyadi);
	}else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		while(tempOgretmen->id != ID)
			tempOgretmen = tempOgretmen->next;
		printf("----\n��retmen ID: %d\n��retmen Ad�: %s\n��retmen Soyad�: %s\n��retmen Unvan: %s\n----\n", ID, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
		printf("��retmen Ad�: "); scanf("%s", tempOgretmen->adi);
		printf("��retmen Soyad�: "); scanf("%s", tempOgretmen->soyadi);
		printf("��retmen Unvan: "); scanf("%s", tempOgretmen->unvan);
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		while(strcmp(tempDers->id, IDC))
			tempDers = tempDers->next;
		printf("----\nDers ID: %s\nDers Ad�: %s\nDers Kredisi: %d\nDers Kontenjan�: %d\nDers ��reticisi ID: %d\n----\n", IDC, tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
		printf("Ders Ad�: "); scanf("%s", tempDers->adi);
		do{
				printf("��retmen ID (yoksa -1): ");
				scanf("%d", &tempDers->ogretmen_id);
		}while(!IS_ID_EXIST(headP2, tempDers->ogretmen_id, NULL, TYPE_OF_OGRETMEN) && tempDers->ogretmen_id != -1);
	}else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = *headP;
		while(tempKayit->id != ID)
			tempKayit = tempKayit->next;
		printf("----\nID: %d\n��renci ID: %d\nDers ID: %s\nKay�t Durumu: ", tempKayit->id, tempKayit->ogrenci_id, tempKayit->ders_id);
		(tempKayit->kayit_durumu == 0) ? printf("BIRAKTI (0)\n") : printf("KAYITLI (1)\n");
		printf("Kay�t Tarihi: %s\n----\n", tempKayit->kayit_tarihi);
		do{
			printf("Kay�t Durumu (0->BIRAKTI, 1->KAYITLI): ");
			scanf("%d", &tempKayit->kayit_durumu);
		}while(tempKayit->kayit_durumu != 0 && tempKayit->kayit_durumu != 1);
	}
	STRUCTTOFILE(headP, choise, path);
}

void DELETEBYID(void **headP, int ID, char *IDC, int choise, char *path){//Ders, ��retmen ve ��renci kay�t silme
	OGRENCI *tempOgrenci, *trashOgrenci;
	OGRETMEN *tempOgretmen, *trashOgretmen;
	DERS *tempDers, *trashDers;
	
	if(choise == TYPE_OF_OGRENCI){//
		tempOgrenci = *headP;
		if(tempOgrenci->id == ID){//�lk eleman silinecek ise
			trashOgrenci = tempOgrenci;
			tempOgrenci = tempOgrenci->next;
			*headP = tempOgrenci;
			free(trashOgrenci);
		}else{//ortada veya son eleman silinecekse
			while(tempOgrenci->next->id != ID)//Bulunamama ihtimali olmad��� i�in o ko�ulu yazmad�k
				tempOgrenci = tempOgrenci->next;
		
			trashOgrenci = tempOgrenci->next;
			if(tempOgrenci->next->next == NULL)//son eleman� siliyorsak
				tempOgrenci->next = NULL;
			else
				tempOgrenci->next = tempOgrenci->next->next;
			free(trashOgrenci);
		}
	}
	else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		if(tempOgretmen->id == ID){//�lk eleman silinecek ise
			trashOgretmen = tempOgretmen;
			tempOgretmen = tempOgretmen->next;
			*headP = tempOgretmen;
			free(trashOgretmen);
		}else{//ortada veya son eleman silinecekse
			while(tempOgretmen->next->id != ID)//Bulunamama ihtimali olmad��� i�in o ko�ulu yazmad�k
				tempOgretmen = tempOgretmen->next;
		
			trashOgretmen = tempOgretmen->next;
			if(tempOgretmen->next->next == NULL)//son eleman� siliyorsak
				tempOgretmen->next = NULL;
			else
				tempOgretmen->next = tempOgretmen->next->next;
			free(trashOgretmen);
		}
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		if(tempDers->id == IDC){//�lk eleman silinecek ise
			trashDers = tempDers;
			tempDers = tempDers->next;
			*headP = tempDers;
			free(trashDers);
		}else{//ortada veya son eleman silinecekse
			while(tempDers->next->id != IDC)//Bulunamama ihtimali olmad��� i�in o ko�ulu yazmad�k
				tempDers = tempDers->next;
		
			trashDers = tempDers->next;
			if(tempDers->next->next == NULL)//son eleman� siliyorsak
				tempDers->next = NULL;
			else
				tempDers->next = tempDers->next->next;
			free(trashDers);
		}
	}
	STRUCTTOFILE(headP, choise, path);
}

void STRUCTTOFILE(void **headP, int choise, char *path){
	FILE *dosya;
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	OGRENCIDERSKAYIT *tempKayit;
	AYARLAR *tempAyarlar;
	
	OPENFILE(&dosya, path, "w");
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = *headP;
		while(tempOgrenci != NULL){
			fprintf(dosya, "\n%d %s %s %d %d", tempOgrenci->id, &tempOgrenci->adi, &tempOgrenci->soyadi, tempOgrenci->toplam_ders, tempOgrenci->toplam_kredi);
			tempOgrenci = tempOgrenci->next;
		}
	}else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		while(tempOgretmen != NULL){
			fprintf(dosya, "\n%d %s %s %s", tempOgretmen->id, &tempOgretmen->adi, &tempOgretmen->soyadi, &tempOgretmen->unvan);
			tempOgretmen = tempOgretmen->next;
		}
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		while(tempDers != NULL){
			fprintf(dosya, "\n%s %s %d %d %d", &tempDers->id, &tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
			tempDers = tempDers->next;
		}
	}else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = *headP;
		while(tempKayit != NULL){
			fprintf(dosya, "\n%d %d %s %d %s", tempKayit->id, tempKayit->ogrenci_id, &tempKayit->ders_id, tempKayit->kayit_durumu, &tempKayit->kayit_tarihi);
			tempKayit = tempKayit->next;
		}
	}else if(choise == TYPE_OF_AYARLAR){
		tempAyarlar = *headP;
		fprintf(dosya, "%d %d %d %d", tempAyarlar->dersSinir, tempAyarlar->krediSinir, tempAyarlar->dersKayitAutoINC, tempAyarlar->dersKontenjan);
	}
	fclose(dosya);
}

int IS_ID_EXIST(void **headP, int ID, char *IDC, int choise){//Varsa ise 1, yoksa 0
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = *headP;
		while(tempOgrenci != NULL && tempOgrenci->id != ID)
			tempOgrenci = tempOgrenci->next;
		if(tempOgrenci != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		while(tempOgretmen != NULL && tempOgretmen->id != ID)
			tempOgretmen = tempOgretmen->next;
		if(tempOgretmen != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		while(tempDers != NULL && strcmp(tempDers->id, IDC) != 0)
			tempDers = tempDers->next;
		if(tempDers != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_DERSOGRETMENKONTROL){
		tempDers = *headP;
		while(tempDers != NULL && tempDers->ogretmen_id != ID)
			tempDers = tempDers->next;
		if(tempDers != NULL) return 1;
		else return 0;
	}
}

void LOADSTRUCT(OGRENCI **ogrPtr, OGRETMEN **ogretmenPtr, DERS **dersPtr, OGRENCIDERSKAYIT **kayitPtr, char *ogrYol, char *ogretYol, char *dersYol, char *kayitYol){
	FILE *dosya;
	void *p;//Warning olu�mamas� i�in olu�turuldu
	
	if(ISFILEEXIST(ogrYol) == FILE_NONE){
		dosya = fopen(ogrYol, "w");
		fclose(dosya);
	}
	p = *ogrPtr;
	FILETOSTRUCT(&p, TYPE_OF_OGRENCI, ogrYol);
	*ogrPtr = p;
		
	if(ISFILEEXIST(ogretYol) == FILE_NONE){
		dosya = fopen(ogretYol, "w");
		fclose(dosya);
	}
	p = *ogretmenPtr;
	FILETOSTRUCT(&p, TYPE_OF_OGRETMEN, ogretYol);
	*ogretmenPtr = p;
	
	if(ISFILEEXIST(dersYol) == FILE_NONE){
		dosya = fopen(dersYol, "w");
		fclose(dosya);
	}
	p = *dersPtr;
	FILETOSTRUCT(&p, TYPE_OF_DERS, dersYol);
	*dersPtr = p;
	
	if(ISFILEEXIST(kayitYol) == FILE_NONE){
		dosya = fopen(kayitYol, "w");
		fclose(dosya);
	}
	p = *kayitPtr;
	FILETOSTRUCT(&p, TYPE_OF_DERSKAYIT, kayitYol);
	*kayitPtr = p;
}

void FILETOSTRUCT(void **headP, int choise, char *path){
	FILE *dosya;
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	OGRENCIDERSKAYIT *tempKayit;
	
	OPENFILE(&dosya, path, "r");
	if(choise == TYPE_OF_OGRENCI){
		if(ISFILEEXIST(path) != FILE_EMPTY)
			while(!feof(dosya)){
				tempOgrenci = (OGRENCI *) malloc(sizeof(OGRENCI));
				fscanf(dosya, "%d %s %s %d %d", &tempOgrenci->id, tempOgrenci->adi, tempOgrenci->soyadi, &tempOgrenci->toplam_ders, &tempOgrenci->toplam_kredi);
				tempOgrenci->next = *headP;
				*headP = tempOgrenci;
			}
	}else if(choise == TYPE_OF_OGRETMEN){
		if(ISFILEEXIST(path) != FILE_EMPTY)
			while(!feof(dosya)){
				tempOgretmen = (OGRETMEN *) malloc(sizeof(OGRETMEN));
				fscanf(dosya, "%d %s %s %s", &tempOgretmen->id, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
				tempOgretmen->next = *headP;
				*headP = tempOgretmen;
			}
	}else if(choise == TYPE_OF_DERS){
		if(ISFILEEXIST(path) != FILE_EMPTY)
			while(!feof(dosya)){
				tempDers = (DERS *) malloc(sizeof(DERS));
				fscanf(dosya, "%s %s %d %d %d", tempDers->id, tempDers->adi, &tempDers->kredi, &tempDers->kontenjan, &tempDers->ogretmen_id);
				tempDers->next = *headP;
				*headP = tempDers;
			}
	}else if(choise == TYPE_OF_DERSKAYIT){
		if(ISFILEEXIST(path) != FILE_EMPTY)
			while(!feof(dosya)){
				tempKayit = (OGRENCIDERSKAYIT *) malloc(sizeof(OGRENCIDERSKAYIT));
				fscanf(dosya, "%d %d %s %d %s", &tempKayit->id, &tempKayit->ogrenci_id, tempKayit->ders_id, &tempKayit->kayit_durumu, tempKayit->kayit_tarihi);
				tempKayit->next = *headP;
				*headP = tempKayit;
			}
	}
	fclose(dosya);
}

int ISFILEEXIST(char *yol){
	FILE *dosya;
	if((dosya = fopen(yol, "r"))){
		fseek(dosya, 0, SEEK_END);
		if(ftell(dosya) == 0){
			fclose(dosya);
			return FILE_EMPTY;//Dosya var ama bo�
		}else{
			fclose(dosya);
			return FILE_EXIST;//Dosya dolu ve var
		}
	}
	else return FILE_NONE;//Dosya yok
}

void OPENFILE(FILE **dosya, char *yol, char *tur){
	if((*dosya = fopen(yol, tur)) == NULL){
		if(tur == "r")
			printf("Okumak istedi�iniz dosya a��l�rken hatayla kar��la��ld�!");
		else
			printf("Dosya a��l�rken hatayla kar��la��ld�!");
		exit(1);
	}
}

void SHOWSTRUCT(void *pointer, int choise){
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	OGRENCIDERSKAYIT *tempKayit;
	
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = pointer;
		printf("��renci Listesi\nID\tADI\tSOYADI\tTOPLAM DERS\tTOPLAM KRED�\n");
		while(tempOgrenci != NULL){
			printf("%d %s %s %d %d\n", tempOgrenci->id, tempOgrenci->adi, tempOgrenci->soyadi, tempOgrenci->toplam_ders, tempOgrenci->toplam_kredi);
			tempOgrenci = tempOgrenci->next;
		}
	}		
	else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = pointer;
		printf("��retmen Listesi\nID\tADI\tSOYADI\tUNVAN\n");
		while(tempOgretmen != NULL){
			printf("%d %s %s %s\n", tempOgretmen->id, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
			tempOgretmen = tempOgretmen->next;
		}
	}
	else if(choise == TYPE_OF_DERS){
		tempDers = pointer;
		printf("Ders Listesi\nID\tADI\tKREDI\tKONTENJAN\tOGRETMEN ID\n");
		while(tempDers != NULL){
			printf("%s %s %d %d %d\n", tempDers->id, tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
			tempDers = tempDers->next;
		}
	}		
	else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = pointer;
		printf("DERS-KAYIT Listesi\nID\tOGRENCI ID\tDERS ID\tKAYIT DURUMU\tKAYIT TAR�H�\n");
		while(tempKayit != NULL){
			printf("%d %d %s ", tempKayit->id, tempKayit->ogrenci_id, tempKayit->ders_id);
			(tempKayit->kayit_durumu == 0) ? printf("BIRAKTI ") : printf("KAYITLI ");
			printf("%s\n", tempKayit->kayit_tarihi);
			tempKayit = tempKayit->next;
		}
	}
}
