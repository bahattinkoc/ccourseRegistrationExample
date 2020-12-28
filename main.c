#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define OGRENCIYOL "D:\\OGRENCILER.txt"
#define OGRETMENYOL "D:\\OGRETMENLER.txt"
#define DERSYOL "D:\\DERSLER.txt"
#define DERSKAYITYOL "D:\\OGRENCIDERSKAYIT.txt"
#define AYARLARYOL "D:\\AYARLAR.txt"

#define TYPE_OF_OGRENCI 0
#define TYPE_OF_OGRETMEN 1
#define TYPE_OF_DERS 2
#define TYPE_OF_DERSKAYIT 3
#define TYPE_OF_DERSOGRETMENKONTROL 4

#define FILE_EXIST 1
#define FILE_NONE 0
#define FILE_EMPTY -1


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
	char kayit_durumu[7];
	char kayit_tarihi[10];
	struct OgrenciDersKayit *next;
}OGRENCIDERSKAYIT;
/////////////

///Summary - PROTOTIPLER
void OPENFILE(FILE **, char *, char *);
void FILETOSTRUCT(void **, int);
void SHOWSTRUCT(void *, int);
void LOADSTRUCT(OGRENCI **, OGRETMEN **, DERS **, OGRENCIDERSKAYIT **);
int FILEISEXIST(char *);
void STRUCTTOFILE(void **, int);
/////////////

int main(){
	setlocale(LC_ALL, "Turkish");
	
	FILE *dosya;
	OGRETMEN *ogretmenHead = NULL;
	OGRENCI *ogrenciHead = NULL;
	DERS *dersHead = NULL;
	OGRENCIDERSKAYIT *ogrenciDersKayitHead;
	int secim = 0, dersSinir, krediSinir, dersKayitAutoINC;
	void *tempVoid;
	
	LOADSTRUCT(&ogrenciHead, &ogretmenHead, &dersHead, &ogrenciDersKayitHead);
	
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
		printf("Seciminiz (çýkmak içim -1): ");
		scanf("%d", &secim);
		
		if(secim == 1){//Öðretmen Ekle
		system("cls");
		printf("***ÖÐRETMEN EKLEME FORMUNA HOÞGELDÝNÝZ***\n\n");
			OGRETMEN *temp = (OGRETMEN *) malloc(sizeof(OGRETMEN));
			do{
				printf("Öðretmen ID: ");
				scanf("%d", &temp->id);
				if(!IS_ID_UNIQUE(&ogretmenHead, temp->id, TYPE_OF_OGRETMEN))
					printf("Var olan bir id girdiniz. Tekrar deneyiniz!\n");
			}while(!IS_ID_UNIQUE(&ogretmenHead, temp->id, TYPE_OF_OGRETMEN));
			printf("Öðretmen Adý: ");
			scanf("%s", &temp->adi);
			printf("Öðretmen Soyadý: ");
			scanf("%s", &temp->soyadi);
			printf("Öðretmen Unvan: ");
			scanf("%s", &temp->unvan);
			temp->next = ogretmenHead;
			ogretmenHead = temp;
			tempVoid = &ogretmenHead;//uyarý almamak için böyle bir aktarma kullandým.
			STRUCTTOFILE(tempVoid, TYPE_OF_OGRETMEN);
		}
		else if(secim == 2){//Öðretmen Sil
			
		}
	}
	
	return 0;
}

void STRUCTTOFILE(void **headP, int choise){
	FILE *dosya;
	if(choise == TYPE_OF_OGRENCI){
		OGRENCI *temp = *headP;
		OPENFILE(&dosya, OGRENCIYOL, "w");
		while(temp != NULL){
			fprintf(dosya, "\n%d %s %s %d %d", temp->id, temp->adi, temp->soyadi, temp->toplam_ders, temp->toplam_kredi);
			temp = temp->next;
		}
	}else if(choise == TYPE_OF_OGRETMEN){
		OGRETMEN *temp = *headP;
		OPENFILE(&dosya, OGRETMENYOL, "w");
		while(temp != NULL){
			fprintf(dosya, "\n%d %s %s %s", temp->id, &temp->adi, &temp->soyadi, &temp->unvan);
			temp = temp->next;
		}
	}else if(choise == TYPE_OF_DERS){
		DERS *temp = *headP;
		OPENFILE(&dosya, DERSYOL, "w");
		while(temp != NULL){
			fprintf(dosya, "\n%d %s %d %d %d", temp->id, temp->adi, temp->kredi, temp->kontenjan, temp->ogretmen_id);
			temp = temp->next;
		}
	}else if(choise == TYPE_OF_DERSKAYIT){
		OGRENCIDERSKAYIT *temp = *headP;
		OPENFILE(&dosya, DERSKAYITYOL, "w");
		while(temp != NULL){
			fprintf(dosya, "\n%d %d %d %s %s", temp->id, temp->ogrenci_id, temp->ders_id, temp->kayit_durumu, temp->kayit_tarihi);
			temp = temp->next;
		}
	}
	fclose(dosya);
}

int IS_ID_UNIQUE(void **headP, int id, int choise){
	if(choise == TYPE_OF_OGRENCI){
		OGRENCI *temp = *headP;
		while(temp != NULL && temp->id != id)
			temp = temp->next;
		if(temp != NULL) return 0;
		else return 1;
	}else if(choise == TYPE_OF_OGRETMEN){
		OGRETMEN *temp = *headP;
		while(temp != NULL && temp->id != id)
			temp = temp->next;
		if(temp != NULL) return 0;
		else return 1;
	}else if(choise == TYPE_OF_DERS){
		DERS *temp = *headP;
		while(temp != NULL && temp->id != id)
			temp = temp->next;
		if(temp != NULL) return 0;
		else return 1;
	}else if(choise == TYPE_OF_DERSOGRETMENKONTROL){
		DERS *temp = *headP;
		while(temp != NULL && temp->ogretmen_id != id)
			temp = temp->next;
		if(temp != NULL) return 0;
		else return 1;
	}
}

void LOADSTRUCT(OGRENCI **ogrenciPointer, OGRETMEN **ogretmenPointer, DERS **dersPointer, OGRENCIDERSKAYIT **dersKayitPointer){
	FILE *dosya;
	void *p;
	
	if(ISFILEEXIST(OGRENCIYOL) == FILE_EXIST){
		p = *ogrenciPointer;
		FILETOSTRUCT(&p, TYPE_OF_OGRENCI);
		*ogrenciPointer = p;
	}
		
	if(ISFILEEXIST(OGRETMENYOL) == FILE_EXIST){
		p = *ogretmenPointer;
		FILETOSTRUCT(&p, TYPE_OF_OGRETMEN);
		*ogretmenPointer = p;
	}
	
	if(ISFILEEXIST(DERSYOL) == FILE_EXIST){
		p = *dersPointer;
		FILETOSTRUCT(&p, TYPE_OF_DERS);
		*dersPointer = p;
	}
	
	if(ISFILEEXIST(DERSKAYITYOL) == FILE_EXIST){
		p = *dersKayitPointer;
		FILETOSTRUCT(&p, TYPE_OF_DERSKAYIT);
		*dersKayitPointer = p;
	}
}

void FILETOSTRUCT(void **headP, int choise){
	FILE *dosya;
	if(choise == TYPE_OF_OGRENCI){
		OPENFILE(&dosya, OGRENCIYOL, "r");
		OGRENCI *temp = NULL;
		while(!feof(dosya)){
			temp = (OGRENCI *) malloc(sizeof(OGRENCI));
			fscanf(dosya, "%d %s %s %d %d", &temp->id, temp->adi, temp->soyadi, &temp->toplam_ders, &temp->toplam_kredi);
			temp->next = *headP;
			*headP = temp;
		}
	}else if(choise == TYPE_OF_OGRETMEN){
		OPENFILE(&dosya, OGRETMENYOL, "r");
		OGRETMEN *temp = NULL;
		while(!feof(dosya)){
			temp = (OGRETMEN *) malloc(sizeof(OGRETMEN));
			fscanf(dosya, "%d %s %s %s", &temp->id, temp->adi, temp->soyadi, temp->unvan);
			temp->next = *headP;
			*headP = temp;
		}
	}else if(choise == TYPE_OF_DERS){
		OPENFILE(&dosya, DERSYOL, "r");
		DERS *temp = NULL;
		while(!feof(dosya)){
			temp = (DERS *) malloc(sizeof(DERS));
			fscanf(dosya, "%d %s %d %d %d", &temp->id, temp->adi, &temp->kredi, &temp->kontenjan, &temp->ogretmen_id);
			temp->next = *headP;
			*headP = temp;
		}
	}else if(choise == TYPE_OF_DERSKAYIT){
		OPENFILE(&dosya, DERSKAYITYOL, "r");
		OGRENCIDERSKAYIT *temp = NULL;
		while(!feof(dosya)){
			temp = (OGRENCIDERSKAYIT *) malloc(sizeof(OGRENCIDERSKAYIT));
			fscanf(dosya, "%d %d %d %s %s", &temp->id, &temp->ogrenci_id, &temp->ders_id, temp->kayit_durumu, temp->kayit_tarihi);
			temp->next = *headP;
			*headP = temp;
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
			printf("Okumak istediðiniz dosya oluþturulamadý! Dosyayý oluþturmak ister misiniz?(E-H) : ");
		else
			printf("Dosya açýlýrken hatayla karþýlaþýldý!");
		exit(1);
	}
}

void SHOWSTRUCT(void *pointer, int choise){
	if(choise == TYPE_OF_OGRENCI){
		OGRENCI *temp = pointer;
		while(temp != NULL){
			printf("%d %s %s %d %d\n", temp->id, temp->adi, temp->soyadi, temp->toplam_ders, temp->toplam_kredi);
			temp = temp->next;
		}
	}		
	else if(choise == TYPE_OF_OGRETMEN){
		OGRETMEN *temp = pointer;
		while(temp != NULL){
			printf("%d %s %s %s\n", temp->id, temp->adi, temp->soyadi, temp->unvan);
			temp = temp->next;
		}
	}
	else if(choise == TYPE_OF_DERS){
		DERS *temp = pointer;
		while(temp != NULL){
			printf("%d %s %d %d %d\n", temp->id, temp->adi, temp->kredi, temp->kontenjan, temp->ogretmen_id);
			temp = temp->next;
		}
	}		
	else if(choise == TYPE_OF_DERSKAYIT){
		OGRENCIDERSKAYIT *temp = pointer;
		while(temp != NULL){
			printf("%d %d %d %s %s\n", temp->id, temp->ogrenci_id, temp->ders_id, temp->kayit_durumu, temp->kayit_tarihi);
			temp = temp->next;
		}
	}
}
