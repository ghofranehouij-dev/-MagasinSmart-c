#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct Produit {
    int id;
    char nom[50];
    float prix;
    int quantite;
    struct Produit* suivant;
} Produit;

typedef struct Client {
    char nom[50];
    float totalAchats;
    struct Client* suivant;
} Client;

typedef struct Vente {
    char client[50];
    char produit[50];
    int quantite;
    float total;
    char date[20];
    struct Vente* suivant;
} Vente;

typedef struct Fournisseur {
    char nom[50];
    char produit[50];
    struct Fournisseur* suivant;
} Fournisseur;


Produit* listeProduits = NULL;
Client* listeClients = NULL;
Vente* listeVentes = NULL;
Fournisseur* listeFournisseurs = NULL;


void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int saisirInt(char* msg) {
    int x;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &x) == 1) {
            return x;
        } else {
            printf(" Veuillez entrer un nombre valide !\n");
            clearInput();
        }
    }
}

float saisirFloat(char* msg) {
    float x;
    while (1) {
        printf("%s", msg);
        if (scanf("%f", &x) == 1) {
            return x;
        } else {
            printf(" Veuillez entrer un nombre valide !\n");
            clearInput();
        }
    }
}

void getDate(char* d) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(d, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}

int saisirIDUnique() {
    int id;

    while(1) {
        id = saisirInt("Entrer ID produit (nombre): ");

        if(id < 0) {
            printf(" ID doit etre positif !\n");
            continue;
        }

        if(idExiste(id)) {
            printf(" ID deja existe ! Essayez un autre.\n");
            continue;
        }

        return id;
    }
}
void ajouterProduit() {
    Produit* p = malloc(sizeof(Produit));
    printf("\n--- Ajouter Produit ---\n");

    do {
       p->id = saisirIDUnique();
        if(idExiste(p->id)) {
            printf(" ID deja existe ! Essayez un autre.\n");
        }
    } while(idExiste(p->id));

    printf("Entrer nom produit: "); scanf("%s", p->nom);
    p->prix = saisirFloat("Entrer prix: ");
    p->quantite = saisirInt("Entrer quantite: ");

    p->suivant = listeProduits;
    listeProduits = p;

    printf(" Produit ajoute avec succes !\n");
}
int idExiste(int id) {
    Produit* p = listeProduits;
    while(p) {
        if(p->id == id)
            return 1;
        p = p->suivant;
    }
    return 0;
}
void afficherProduits() {
    printf("\n--- Liste des Produits ---\n");
    Produit* p = listeProduits;
    if(!p) printf("Aucun produit disponible !\n");
    while(p) {
        printf("ID:%d | %s | %.2f DT | Stock:%d\n", p->id, p->nom, p->prix, p->quantite);
        p = p->suivant;
    }
}

Produit* chercherProduit(int id) {
    Produit* p = listeProduits;
    while(p) {
        if(p->id == id) return p;
        p = p->suivant;
    }
    return NULL;
}


Client* chercherClient(char nom[]) {
    Client* c = listeClients;
    while(c) {
        if(strcmp(c->nom, nom)==0) return c;
        c = c->suivant;
    }
    return NULL;
}

Client* ajouterClient(char nom[]) {
    Client* c = chercherClient(nom);
    if(c) return c;

    c = malloc(sizeof(Client));
    strcpy(c->nom, nom);
    c->totalAchats = 0;

    c->suivant = listeClients;
    listeClients = c;

    printf(" Nouveau client ajoute !\n");
    return c;
}


void ajouterFournisseur() {
    Fournisseur* f = malloc(sizeof(Fournisseur));
    printf("\n--- Ajouter Fournisseur ---\n");
    printf("Nom fournisseur: "); scanf("%s", f->nom);
    printf("Produit fourni: "); scanf("%s", f->produit);

    f->suivant = listeFournisseurs;
    listeFournisseurs = f;

    printf(" Fournisseur ajoute !\n");
}

void afficherFournisseurs() {
    printf("\n--- Liste Fournisseurs ---\n");
    Fournisseur* f = listeFournisseurs;
    if(!f) printf("Aucun fournisseur !\n");
    while(f) {
        printf("%s -> %s\n", f->nom, f->produit);
        f = f->suivant;
    }
}


void acheterProduit() {
    int id,qte;
    char nom[50];

    printf("\n--- Achat Produit ---\n");
    printf("Entrer nom client: "); scanf("%s", nom);
    Client* c = ajouterClient(nom);

    id = saisirInt("Entrer ID produit: ");
    Produit* p = chercherProduit(id);

    if(!p) {
        printf(" Produit introuvable !\n");
        return;
    }

    qte = saisirInt("Entrer quantite: ");

    if(qte > p->quantite) {
        printf("Stock insuffisant !\n");
        return;
    }

    p->quantite -= qte;

    Vente* v = malloc(sizeof(Vente));
    strcpy(v->client, nom);
    strcpy(v->produit, p->nom);
    v->quantite = qte;
    v->total = qte * p->prix;
    getDate(v->date);

    v->suivant = listeVentes;
    listeVentes = v;

    c->totalAchats += v->total;

    printf("Achat reussi ! Total: %.2f DT | Date: %s\n", v->total, v->date);
}


void historiqueVentes() {
    printf("\n--- Historique des Ventes ---\n");
    Vente* v = listeVentes;
    if(!v) printf("Aucune vente !\n");
    while(v) {
        printf("%s | %s | %d | %.2f DT | %s\n",
        v->client, v->produit, v->quantite, v->total, v->date);
        v = v->suivant;
    }
}

void historiqueClient() {
    char nom[50];
    printf("\n--- Historique Client ---\n");
    printf("Entrer nom client: "); scanf("%s", nom);

    Vente* v = listeVentes;
    float total=0;

    while(v) {
        if(strcmp(v->client, nom)==0) {
            printf("%s | %s | %d | %.2f DT | %s\n",
            v->client,v->produit,v->quantite,v->total,v->date);
            total+=v->total;
        }
        v=v->suivant;
    }

    printf("Total achats = %.2f DT\n", total);
}


void topClient() {
    Client* c = listeClients;
    Client* best=NULL;

    while(c) {
        if(!best || c->totalAchats > best->totalAchats)
            best=c;
        c=c->suivant;
    }

    if(best)
        printf(" Top client: %s (%.2f DT)\n", best->nom, best->totalAchats);
}

// ================= FICHIERS =================

void saveProduits() {
    FILE* f=fopen("produits.txt","w");
    Produit* p=listeProduits;
    while(p){
        fprintf(f,"%d %s %f %d\n",p->id,p->nom,p->prix,p->quantite);
        p=p->suivant;
    }
    fclose(f);
}

void loadProduits() {
    FILE* f=fopen("produits.txt","r");
    if(!f) return;

    while(1){
        Produit* p=malloc(sizeof(Produit));
        if(fscanf(f,"%d %s %f %d",&p->id,p->nom,&p->prix,&p->quantite)!=4)
            break;
        p->suivant=listeProduits;
        listeProduits=p;
    }
    fclose(f);
}

void saveClients(){
    FILE* f=fopen("clients.txt","w");
    Client* c=listeClients;
    while(c){
        fprintf(f,"%s %f\n",c->nom,c->totalAchats);
        c=c->suivant;
    }
    fclose(f);
}

void saveVentes(){
    FILE* f=fopen("ventes.txt","w");
    Vente* v=listeVentes;
    while(v){
        fprintf(f,"%s %s %d %f %s\n",
        v->client,v->produit,v->quantite,v->total,v->date);
        v=v->suivant;
    }
    fclose(f);
}

void saveFournisseurs(){
    FILE* f=fopen("fournisseurs.txt","w");
    Fournisseur* fo=listeFournisseurs;
    while(fo){
        fprintf(f,"%s %s\n",fo->nom,fo->produit);
        fo=fo->suivant;
    }
    fclose(f);
}

void menu(){
    int ch;
    do{
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Ajouter Produit\n");
        printf("2. Afficher Produits\n");
        printf("3. Acheter Produit\n");
        printf("4. Historique Ventes\n");
        printf("5. Historique Client\n");
        printf("6. Ajouter Fournisseur\n");
        printf("7. Afficher Fournisseurs\n");
        printf("8. Top Client\n");
        printf("0. Quitter\n");
        printf("===================================\n");

        ch = saisirInt("Choisir votre choix (0-8): ");

        if(ch < 0 || ch > 8) {
            printf(" Choix non disponible ! Reessayez.\n");
            continue;
        }

        switch(ch){
            case 1: ajouterProduit(); break;
            case 2: afficherProduits(); break;
            case 3: acheterProduit(); break;
            case 4: historiqueVentes(); break;
            case 5: historiqueClient(); break;
            case 6: ajouterFournisseur(); break;
            case 7: afficherFournisseurs(); break;
            case 8: topClient(); break;
            case 0:
                saveProduits();
                saveClients();
                saveVentes();
                saveFournisseurs();
                printf("Donnees sauvegardees avec succes !\n");
                break;
        }

    }while(ch!=0);
}


int main(){
    loadProduits();
    printf("\n===== BIENVENUE DANS MAGASIN SMART =====\n");
    menu();
    printf("\nMerci d'avoir utilise le programme \n");
    return 0;
}
