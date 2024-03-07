#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_ETUDIANTS 100
#define MAX_PRESENCES 3100 // 100 étudiants * 31 jours
#define MAX_UTILISATEURS 20

typedef struct
{
    char nom[50];
    char prenom[50];
    char classe[20];
    int numeroEtudiant;
    int estBloque;
} Etudiant;

typedef struct
{
    int numeroEtudiant;
    time_t dateHeure;
    int estPresent;
} Presence;

typedef struct
{
    char username[50];
    char password[50];
    int estAdmin;
} Utilisateur;

// Déclarations des prototypes de fonctions
void menuAdmin();
void menuApprenant();
void gestionEtudiants();
void generationFichiers();
void nombreMinutesAbsence();
void mesMessages();
void gestionUtilisateurs();
void ajouterUtilisateur();
void lireUtilisateurs();
int authentifier();
void ajouterEtudiant();
void supprimerEtudiant();
void listerEtudiants();
void envoyerMessage();
void marquerPresence();
void supprimerUtilisateur();
void listerUtilisateurs();
void afficherClasses();
void afficherListeEtudiants(const char *nomFichier); // Prototype ajouté
void sousMenuPresences();

Etudiant etudiants[MAX_ETUDIANTS];
int nbEtudiants = 0;

Presence presences[MAX_PRESENCES];
int nbPresences = 0;

Utilisateur utilisateurs[MAX_UTILISATEURS];
int nbUtilisateurs = 0;

void lireUtilisateurs()
{
    FILE *fichier = fopen("utilisateurs.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        char username[50], password[50];
        int estAdmin;
        sscanf(ligne, "%[^,],%[^,],%d", username, password, &estAdmin);

        strcpy(utilisateurs[nbUtilisateurs].username, username);
        strcpy(utilisateurs[nbUtilisateurs].password, password);
        utilisateurs[nbUtilisateurs].estAdmin = estAdmin;
        nbUtilisateurs++;
    }

    fclose(fichier);
}

int authentifier()
{
    char username[50], *password;
    printf("Nom d'utilisateur : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    // Vérification de la validité du nom d'utilisateur
    while (strlen(username) == 0 || strspn(username, " ") == strlen(username) || isspace((unsigned char)username[0]))
    {
        printf("Le nom d'utilisateur ne peut pas être vide ou ne peut contenir que des espaces.\n");
        printf("Nom d'utilisateur : ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;
    }

    // Utilisation de la fonction getpass pour masquer le mot de passe
    password = getpass("Mot de passe : ");

    // Vérification des informations d'authentification
    for (int i = 0; i < nbUtilisateurs; i++)
    {
        if (strcmp(utilisateurs[i].username, username) == 0 && strcmp(utilisateurs[i].password, password) == 0)
        {
            return utilisateurs[i].estAdmin;
        }
    }

    // Si l'authentification échoue
    printf("Authentification échouée.\n");
    return -1;
}

void menuAdmin()
{
    int choix;
    do
    {
        printf("\n===== Menu Admin =====\n");
        printf("1. Gestion des etudiants\n");
        printf("2. Generation de fichiers\n");
        printf("3. Marquer les presences\n");
        printf("4. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix)
        {
        // case 1:
        //     gestionEtudiants();
        //     break;
        // case 2:
        //     generationFichiers();
        //     break;
        case 3:
            sousMenuPresences(); // Appel du sous-menu pour marquer les présences
            break;
        case 4:
            printf("Deconnexion...\n");
            break;
        default:
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 4.\n");
        }

    } while (choix != 4);
}

void afficherListeEtudiants(const char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    char ligne[100];
    printf("\n===== Liste des Etudiants =====\n");
    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        printf("%s", ligne);
    }

    fclose(fichier);
}

void sousMenuPresences()
{
    int choixClasse;
    do
    {
        printf("\n===== Choisissez une classe =====\n");
        printf("1. Licence 1\n");
        printf("2. Licence 2\n");
        printf("Votre choix : ");
        scanf("%d", &choixClasse);

        switch (choixClasse)
        {
        case 1:
            // Marquer les présences pour Licence 1
            printf("Vous avez choisi Licence 1.\n");
            afficherListeEtudiants("licence1.txt");
            printf("Veuillez choisir un étudiant: ");       
            int numEtudiant, estPresent;
            scanf("%d", &numEtudiant);
            printf("Present ? (0/1) : ");
            scanf("%d", &estPresent);

            Presence p;
            p.numeroEtudiant = numEtudiant;
            p.dateHeure = time(NULL);
            p.estPresent = estPresent;

            presences[nbPresences++] = p;
            FILE *f = fopen("presences.txt", "a");
            char presence[30];
            sprintf(&presence,"%d", numEtudiant);
            sprintf(f, &presence);
            printf("Presence marquee avec succes.\n");
            // Ajoutez ici le code pour marquer les présences pour Licence 1
            break;
        case 2:
            // Marquer les présences pour Licence 2
            printf("Vous avez choisi Licence 2.\n");
            afficherListeEtudiants("licence2.txt");
            // Ajoutez ici le code pour marquer les présences pour Licence 2
            break;
        default:
            printf("Choix invalide. Veuillez entrer 1 ou 2.\n");
        }

    } while (choixClasse != 1 && choixClasse != 2);
}

void menuApprenant()
{
    int choix;
    do
    {
        printf("\n===== Menu Apprenant =====\n");
        printf("1. Marquer ma presence\n");
        printf("2. Nombre de minutes d'absence\n");
        printf("3. Mes messages\n");
        printf("4. Deconnexion\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

    } while (choix != 4);
}

void menuAuthentification()
{
    int estAdmin = authentifier();
    if (estAdmin == -1)
    {
        return;
    }
    else if (estAdmin == 1)
    {
        menuAdmin();
    }
    else
    {
        menuApprenant();
    }
}

int main()
{
    lireUtilisateurs(); // Lire les utilisateurs depuis le fichier

    menuAuthentification();
    return 0;
}
