#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

#define TYPE_OF_OGRENCI 0
#define TYPE_OF_OGRETMEN 1
#define TYPE_OF_DERS 2
#define TYPE_OF_DERSKAYIT 3
#define TYPE_OF_DERSOGRETMENKONTROL 4

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
	int id;
	char adi[20];
	int kredi;
	int kontenjan;
	int ogretmen_id;
	struct Ders *next;
}DERS;

typedef struct OgrenciDersKayit{
	int id;//AUTO INCREMENT
	int ogrenci_id;
	int ders_id;
	int kayit_durumu;
	char kayit_tarihi[10];
	struct OgrenciDersKayit *next;
}OGRENCIDERSKAYIT;
/////////////

///Summary - PROTOTIPLER
void OPENFILE(FILE **, char *, char *);
void FILETOSTRUCT(void **, int, char *);
void SHOWSTRUCT(void *, int);
void LOADSTRUCT(OGRENCI **, OGRETMEN **, DERS **, OGRENCIDERSKAYIT **, char *, char *, char *, char *);
int FILEISEXIST(char *);
int IS_ID_EXIST(void **, int, int);
void STRUCTTOFILE(void **, int, char *);
void DELETEBYID(void **, int, int, char *);
void UPDATEBYID(void **, int, int, char *);
/////////////

int main(){
	setlocale(LC_ALL, "Turkish");
	
	FILE *dosya;
	OGRETMEN *ogretmenHead = NULL, *ogretmenTemp;
	OGRENCI *ogrenciHead = NULL, *ogrenciTemp;
	DERS *dersHead = NULL, *dersTemp;
	OGRENCIDERSKAYIT *dersKayitHead, *dersKayitTemp;
	int secim = 0, dersSinir, krediSinir, dersKayitAutoINC, tempNumber;
	void *tempVoid, *tempVoid2;
	char *OGRENCIYOL, *OGRETMENYOL, *DERSYOL, *DERSKAYITYOL, *AYARLARYOL;
	
	OGRENCIYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	OGRETMENYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	DERSYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	DERSKAYITYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	AYARLARYOL = (char *) malloc(PATH_SIZE * sizeof(char));
	
	/*printf("***ÖN AYAR SAYFASINA HOÞGELDÝNÝZ***\n\n");
	printf("Öðrenci Dosyasý Yolunu Giriniz (OGRENCILER.txt): "); scanf("%s", OGRENCIYOL);
	printf("Öðretmen Dosyasý Yolunu Giriniz (OGRETMENLER.txt): "); scanf("%s", OGRETMENYOL);
	printf("Ders Dosyasý Yolunu Giriniz (DERSLER.txt): "); scanf("%s", DERSYOL);
	printf("Ders-Kayýt Dosyasý Yolunu Giriniz (OGRENCIDERSKAYIT.txt): "); scanf("%s", DERSKAYITYOL);
	printf("Ayarlar Dosyasý Yolunu Giriniz (AYARLAR.txt): "); scanf("%s", AYARLARYOL);*/
	OGRENCIYOL = "OGRENCILER.txt";
	OGRETMENYOL = "OGRETMENLER.txt";
	DERSYOL = "DERSLER.txt";
	DERSKAYITYOL = "OGRENCIDERSKAYIT.txt";
	AYARLARYOL = "AYARLAR.txt";
	
	LOADSTRUCT(&ogrenciHead, &ogretmenHead, &dersHead, &dersKayitHead, OGRENCIYOL, OGRETMENYOL, DERSYOL, DERSKAYITYOL);
	
	if(ISFILEEXIST(AYARLARYOL) != FILE_EXIST){//AYAR DOSYASI KONTROLÜ
		if(ISFILEEXIST(AYARLARYOL) == FILE_NONE)//Dosya var deðilse ve dosya yoktuysa oluþtur
			if((dosya = fopen(AYARLARYOL, "w")) == NULL){
				printf("Ayarlar dosyasý olþturulurken hata meydana geldi! Çýkýþ yapýlýyor..");
				exit(1);
			}else fclose(dosya);
			
		OPENFILE(&dosya, AYARLARYOL, "w");
		printf("Görünüþe bakýlýrsa Ayarlar dosyan oluþturulmamýþ veya içerisi boþ.\nHadi beraber oluþturalým.\n1-Ders Sýnýrý: ");
		scanf("%d", &dersSinir);
		printf("2-Kredi Sýnýr: ");
		scanf("%d", &krediSinir);
		
		fprintf(dosya, "%d %d", dersSinir, krediSinir);
		fclose(dosya);
	}else{//Dosya varsa
		OPENFILE(&dosya, AYARLARYOL, "r");
		fscanf(dosya, "%d %d", &dersSinir, &krediSinir);
		fclose(dosya);
	}
	
	while(secim != -1){
		system("cls");
		printf("***WELCOME TO MY HOMEWORK***\n\n");
		
		printf("---Öðretim Üyesi Ýþlemleri---\n1-Ekle\n2-Sil\n3-Güncelle\n---\n");
		printf("---Ders Ýþlemleri---\n4-Ekle\n5-Sil\n6-Güncelle\n---\n");
		printf("---Öðrenci Ýþlemleri---\n7-Ekle\n8-Sil\n9-Güncelle\n---\n");
		printf("---Öðrenci-Ders Kayýt Ýþlemleri---\n10-Ekle\n11-Güncelle\n---\n");
		printf("Seciminiz (çýkmak içim -1): ");
		scanf("%d", &secim);
		
		if(secim == 1){//Öðretmen Ekle
			system("cls");
			printf("***ÖÐRETMEN EKLEME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			ogretmenTemp = (OGRETMEN *) malloc(sizeof(OGRETMEN));
			tempVoid = &ogretmenHead;//uyarý almamak için böyle bir aktarma kullandým.
			do{
				printf("Öðretmen ID: ");
				scanf("%d", &ogretmenTemp->id);
				if(IS_ID_EXIST(tempVoid, ogretmenTemp->id, TYPE_OF_OGRETMEN) || ogretmenTemp->id < 0)
					printf("Var olan bir ID veya negatif bir sayý girdiniz. Tekrar deneyiniz!\n");
			}while(IS_ID_EXIST(tempVoid, ogretmenTemp->id, TYPE_OF_OGRETMEN) || ogretmenTemp->id < 0);
			printf("Öðretmen Adý: ");
			scanf("%s", &ogretmenTemp->adi);
			printf("Öðretmen Soyadý: ");
			scanf("%s", &ogretmenTemp->soyadi);
			printf("Öðretmen Unvan: ");
			scanf("%s", &ogretmenTemp->unvan);
			ogretmenTemp->next = ogretmenHead;
			ogretmenHead = ogretmenTemp;
			STRUCTTOFILE(tempVoid, TYPE_OF_OGRETMEN, OGRETMENYOL);
		}
		else if(secim == 2){//Öðretmen Sil
			system("cls");
			printf("***ÖÐRETMEN SÝLME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			SHOWSTRUCT(ogretmenHead, TYPE_OF_OGRETMEN);
			tempVoid = &ogretmenHead;
			do{
				printf("\nÖðretmen ID (çýkýþ için -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRETMEN) && tempNumber != -1)
					printf("Sistemde kayýtlý olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRETMEN) && tempNumber != -1);

			tempVoid = &dersHead;
			if(IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_DERSOGRETMENKONTROL))
				printf("Ders veren bir öðretmen silmek istiyorsunuz. Bunun için önce dersi silmelisiniz!\nDevam etmek için bir tuþa basýnýz...");
			else if(tempNumber != -1){
				tempVoid = &ogretmenHead;
				DELETEBYID(tempVoid, tempNumber, TYPE_OF_OGRETMEN, OGRETMENYOL);
				printf("Silme iþlemi baþarýlý!\nDevam etmek için bir tuþa basýnýz...");
			}
			getch();
		}
		else if(secim == 3){//Öðretmen Güncelle
			system("cls");
			printf("***ÖÐRETMEN GÜNCELLEME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			SHOWSTRUCT(ogretmenHead, TYPE_OF_OGRETMEN);
			tempVoid = &ogretmenHead;
			do{
				printf("\nÖðretmen ID (çýkýþ için -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRETMEN))
					printf("Sistemde kayýtlý olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRETMEN) && tempNumber != -1);
			
			if(tempNumber != -1){
				UPDATEBYID(tempVoid, tempNumber, TYPE_OF_OGRETMEN, OGRETMENYOL);
				printf("Güncelleme iþlemi baþarýlý!\nDevam etmek için bir tuþa basýnýz...");
			}
			getch();
		}
		else if(secim == 7){//Öðrenci Ekleme
			system("cls");
			printf("***ÖÐRENCÝ EKLEME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			ogrenciTemp = (OGRENCI *) malloc(sizeof(OGRENCI));
			tempVoid = &ogrenciHead;//uyarý almamak için böyle bir aktarma kullandým.
			do{
				printf("Öðrenci ID: ");
				scanf("%d", &ogrenciTemp->id);
				if(IS_ID_EXIST(tempVoid, ogrenciTemp->id, TYPE_OF_OGRENCI) || ogrenciTemp->id < 0)
					printf("Var olan bir ID veya negatif bir sayý girdiniz. Tekrar deneyiniz!\n");
			}while(IS_ID_EXIST(tempVoid, ogrenciTemp->id, TYPE_OF_OGRENCI) || ogrenciTemp->id < 0);
			printf("Öðrenci Adý: ");
			scanf("%s", &ogrenciTemp->adi);
			printf("Öðrenci Soyadý: ");
			scanf("%s", &ogrenciTemp->soyadi);
			ogrenciTemp->toplam_ders = ogrenciTemp->toplam_kredi = 0;//Varsayýlan olarak 0
			ogrenciTemp->next = ogrenciHead;
			ogrenciHead = ogrenciTemp;
			STRUCTTOFILE(tempVoid, TYPE_OF_OGRENCI, OGRENCIYOL);
		}
		else if(secim == 8){//Öðrenci Sil
			system("cls");
			printf("***ÖÐRENCÝ SÝLME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			SHOWSTRUCT(ogrenciHead, TYPE_OF_OGRENCI);
			tempVoid = &ogrenciHead;
			do{
				printf("\nÖðrenci ID (çýkýþ için -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRENCI) && tempNumber != -1)
					printf("Sistemde kayýtlý olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRENCI) && tempNumber != -1);
			
			if(tempNumber != -1){
				DELETEBYID(tempVoid, tempNumber, TYPE_OF_OGRENCI, OGRENCIYOL);
				printf("Silme iþlemi baþarýlý!\nDevam etmek için bir tuþa basýnýz...");
			}
			getch();
		}
		else if(secim == 9){//Öðrenci Güncelle
			system("cls");
			printf("***ÖÐRENCÝ GÜNCELLEME SAYFASINA HOÞGELDÝNÝZ***\n\n");
			SHOWSTRUCT(ogrenciHead, TYPE_OF_OGRENCI);
			tempVoid = &ogrenciHead;
			do{
				printf("\nÖðrenci ID (çýkýþ için -1): ");
				scanf("%d", &tempNumber);
				if(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRENCI))
					printf("Sistemde kayýtlý olmayan bir ID girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_EXIST(tempVoid, tempNumber, TYPE_OF_OGRENCI) && tempNumber != -1);
			
			if(tempNumber != -1){
				UPDATEBYID(tempVoid, tempNumber, TYPE_OF_OGRENCI, OGRENCIYOL);
				printf("Güncelleme iþlemi baþarýlý!\nDevam etmek için bir tuþa basýnýz...");
			}
			getch();
		}
	}
	
	return 0;
}



void UPDATEBYID(void **headP, int ID, int choise, char *path){
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	OGRENCIDERSKAYIT *tempKayit;
	
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = *headP;
		while(tempOgrenci->id != ID)
			tempOgrenci = tempOgrenci->next;
		printf("----\nÖðrenci ID: %d\nÖðrenci Adý: %s\nÖðrenci Soyadý: %s\nÖðrenci Toplam Ders: %d\nÖðrenci Toplam Kredi: %d\n----\n", ID, tempOgrenci->adi, tempOgrenci->soyadi, tempOgrenci->toplam_ders, tempOgrenci->toplam_kredi);
		printf("Öðrenci Adý: "); scanf("%s", tempOgrenci->adi);
		printf("Öðrenci Soyadý: "); scanf("%s", tempOgrenci->soyadi);
	}else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		while(tempOgretmen->id != ID)
			tempOgretmen = tempOgretmen->next;
		printf("----\nÖðretmen ID: %d\nÖðretmen Adý: %s\nÖðretmen Soyadý: %s\nÖðretmen Unvan: %s\n----\n", ID, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
		printf("Öðretmen Adý: "); scanf("%s", tempOgretmen->adi);
		printf("Öðretmen Soyadý: "); scanf("%s", tempOgretmen->soyadi);
		printf("Öðretmen Unvan: "); scanf("%s", tempOgretmen->unvan);
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		while(tempDers->id != ID)
			tempDers = tempDers->next;
		printf("----\nDers ID: %d\nDers Adý: %s\nDers Kredisi: %d\nDers Kontenjaný: %d\nDers Öðreticisi ID: %d\n----\n", ID, tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
		printf("Ders Adý: "); scanf("%s", tempDers->adi);
		//Öðretmen ID güncelleme yapýlabilir
	}else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = *headP;
		while(tempKayit->id != ID)
			tempKayit = tempKayit->next;
		printf("----\nID: %d\nÖðrenci ID: %d\nDers ID: %d\nKayýt Durumu: ", tempKayit->id, tempKayit->ogrenci_id, tempKayit->ders_id);
		(tempKayit->kayit_durumu == 0) ? printf("BIRAKTI (0)\n") : printf("KAYITLI (1)\n");
		printf("Kayýt Tarihi: %s\n----\n", tempKayit->kayit_tarihi);
		do{
			printf("Kayýt Durumu (0->BIRAKTI, 1->KAYITLI): ");
			scanf("%d", &tempKayit->kayit_durumu);
		}while(tempKayit->kayit_durumu != 0 && tempKayit->kayit_durumu != 1);
	}
	STRUCTTOFILE(headP, choise, path);
}

void DELETEBYID(void **headP, int ID, int choise, char *path){//Ders, Öðretmen ve Öðrenci kayýt silme
	OGRENCI *tempOgrenci, *trashOgrenci;
	OGRETMEN *tempOgretmen, *trashOgretmen;
	DERS *tempDers, *trashDers;
	
	if(choise == TYPE_OF_OGRENCI){//
		tempOgrenci = *headP;
		if(tempOgrenci->id == ID){//Ýlk eleman silinecek ise
			trashOgrenci = tempOgrenci;
			tempOgrenci = tempOgrenci->next;
			*headP = tempOgrenci;
			free(trashOgrenci);
		}else{//ortada veya son eleman silinecekse
			while(tempOgrenci->next->id != ID)//Bulunamama ihtimali olmadýðý için o koþulu yazmadýk
				tempOgrenci = tempOgrenci->next;
		
			trashOgrenci = tempOgrenci->next;
			if(tempOgrenci->next->next == NULL)//son elemaný siliyorsak
				tempOgrenci->next = NULL;
			else
				tempOgrenci->next = tempOgrenci->next->next;
			free(trashOgrenci);
		}
	}
	else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		if(tempOgretmen->id == ID){//Ýlk eleman silinecek ise
			trashOgretmen = tempOgretmen;
			tempOgretmen = tempOgretmen->next;
			*headP = tempOgretmen;
			free(trashOgretmen);
		}else{//ortada veya son eleman silinecekse
			while(tempOgretmen->next->id != ID)//Bulunamama ihtimali olmadýðý için o koþulu yazmadýk
				tempOgretmen = tempOgretmen->next;
		
			trashOgretmen = tempOgretmen->next;
			if(tempOgretmen->next->next == NULL)//son elemaný siliyorsak
				tempOgretmen->next = NULL;
			else
				tempOgretmen->next = tempOgretmen->next->next;
			free(trashOgretmen);
		}
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		if(tempDers->id == ID){//Ýlk eleman silinecek ise
			trashDers = tempDers;
			tempDers = tempDers->next;
			*headP = tempDers;
			free(trashDers);
		}else{//ortada veya son eleman silinecekse
			while(tempDers->next->id != ID)//Bulunamama ihtimali olmadýðý için o koþulu yazmadýk
				tempDers = tempDers->next;
		
			trashDers = tempDers->next;
			if(tempDers->next->next == NULL)//son elemaný siliyorsak
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
			fprintf(dosya, "\n%d %s %d %d %d", tempDers->id, &tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
			tempDers = tempDers->next;
		}
	}else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = *headP;
		while(tempKayit != NULL){
			fprintf(dosya, "\n%d %d %d %d %s", tempKayit->id, tempKayit->ogrenci_id, tempKayit->ders_id, tempKayit->kayit_durumu, &tempKayit->kayit_tarihi);
			tempKayit = tempKayit->next;
		}
	}
	fclose(dosya);
}

int IS_ID_EXIST(void **headP, int id, int choise){//Varsa ise 1, yoksa 0
	OGRENCI *tempOgrenci;
	OGRETMEN *tempOgretmen;
	DERS *tempDers;
	
	if(choise == TYPE_OF_OGRENCI){
		tempOgrenci = *headP;
		while(tempOgrenci != NULL && tempOgrenci->id != id)
			tempOgrenci = tempOgrenci->next;
		if(tempOgrenci != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = *headP;
		while(tempOgretmen != NULL && tempOgretmen->id != id)
			tempOgretmen = tempOgretmen->next;
		if(tempOgretmen != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_DERS){
		tempDers = *headP;
		while(tempDers != NULL && tempDers->id != id)
			tempDers = tempDers->next;
		if(tempDers != NULL) return 1;
		else return 0;
	}else if(choise == TYPE_OF_DERSOGRETMENKONTROL){
		tempDers = *headP;
		while(tempDers != NULL && tempDers->ogretmen_id != id)
			tempDers = tempDers->next;
		if(tempDers != NULL) return 1;
		else return 0;
	}
}

void LOADSTRUCT(OGRENCI **ogrPtr, OGRETMEN **ogretmenPtr, DERS **dersPtr, OGRENCIDERSKAYIT **kayitPtr, char *ogrYol, char *ogretYol, char *dersYol, char *kayitYol){
	FILE *dosya;
	void *p;//Warning oluþmamasý için oluþturuldu
	
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
		while(!feof(dosya)){
			tempOgrenci = (OGRENCI *) malloc(sizeof(OGRENCI));
			fscanf(dosya, "%d %s %s %d %d", &tempOgrenci->id, tempOgrenci->adi, tempOgrenci->soyadi, &tempOgrenci->toplam_ders, &tempOgrenci->toplam_kredi);
			tempOgrenci->next = *headP;
			*headP = tempOgrenci;
		}
	}else if(choise == TYPE_OF_OGRETMEN){
		while(!feof(dosya)){
			tempOgretmen = (OGRETMEN *) malloc(sizeof(OGRETMEN));
			fscanf(dosya, "%d %s %s %s", &tempOgretmen->id, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
			tempOgretmen->next = *headP;
			*headP = tempOgretmen;
		}
	}else if(choise == TYPE_OF_DERS){
		while(!feof(dosya)){
			tempDers = (DERS *) malloc(sizeof(DERS));
			fscanf(dosya, "%d %s %d %d %d", &tempDers->id, tempDers->adi, &tempDers->kredi, &tempDers->kontenjan, &tempDers->ogretmen_id);
			tempDers->next = *headP;
			*headP = tempDers;
		}
	}else if(choise == TYPE_OF_DERSKAYIT){
		while(!feof(dosya)){
			tempKayit = (OGRENCIDERSKAYIT *) malloc(sizeof(OGRENCIDERSKAYIT));
			fscanf(dosya, "%d %d %d %d %s", &tempKayit->id, &tempKayit->ogrenci_id, &tempKayit->ders_id, &tempKayit->kayit_durumu, tempKayit->kayit_tarihi);
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
			return FILE_EMPTY;//Dosya var ama boþ
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
			printf("Okumak istediðiniz dosya açýlýrken hatayla karþýlaþýldý!");
		else
			printf("Dosya açýlýrken hatayla karþýlaþýldý!");
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
		printf("Öðrenci Listesi\nID\tADI\tSOYADI\tTOPLAM DERS\tTOPLAM KREDÝ\n");
		while(tempOgrenci != NULL){
			printf("%d %s %s %d %d\n", tempOgrenci->id, tempOgrenci->adi, tempOgrenci->soyadi, tempOgrenci->toplam_ders, tempOgrenci->toplam_kredi);
			tempOgrenci = tempOgrenci->next;
		}
	}		
	else if(choise == TYPE_OF_OGRETMEN){
		tempOgretmen = pointer;
		printf("Öðretmen Listesi\nID\tADI\tSOYADI\tUNVAN\n");
		while(tempOgretmen != NULL){
			printf("%d %s %s %s\n", tempOgretmen->id, tempOgretmen->adi, tempOgretmen->soyadi, tempOgretmen->unvan);
			tempOgretmen = tempOgretmen->next;
		}
	}
	else if(choise == TYPE_OF_DERS){
		tempDers = pointer;
		printf("Ders Listesi\nID\tADI\tKREDI\tKONTENJAN\tOGRETMEN ID\n");
		while(tempDers != NULL){
			printf("%d %s %d %d %d\n", tempDers->id, tempDers->adi, tempDers->kredi, tempDers->kontenjan, tempDers->ogretmen_id);
			tempDers = tempDers->next;
		}
	}		
	else if(choise == TYPE_OF_DERSKAYIT){
		tempKayit = pointer;
		printf("DERS-KAYIT Listesi\nID\tOGRENCI ID\tDERS ID\tKAYIT DURUMU\tKAYIT TARÝHÝ\n");
		while(tempKayit != NULL){
			printf("%d %d %d ", tempKayit->id, tempKayit->ogrenci_id, tempKayit->ders_id);
			(tempKayit->kayit_durumu == 0) ? printf("BIRAKTI ") : printf("KAYITLI ");
			printf("%s\n", tempKayit->kayit_tarihi);
			tempKayit = tempKayit->next;
		}
	}
}
