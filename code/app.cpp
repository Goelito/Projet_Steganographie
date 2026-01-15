#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <codecvt>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

using namespace std;
using namespace cv;

int decode(string img_path);
int encode(string img_path);

static void bandeau(){
    cout<<"--------------------------------------------------------"<<endl;
    cout<<"| Application Stéganographaphie | Encodage et décodage |"<<endl;
    cout<<"--------------------------------------------------------"<<endl;
    cout<<"Pour encoder ou décoder, veuillez utiliser un fichier au format PNG."<<endl<<endl;
}

int menu(){
    system("cls");
    bandeau();
    string choix;
    string img_path;
    cout<<"Décoder : 0 | Encoder : 1"<<endl;
    cout<<"Saisie : ";
    cin>>choix;
    while((choix != "0") && (choix != "1")){
        system("cls");
        bandeau();
        cout<<"Décoder : 0 | Encoder : 1"<<endl;
        cout<<"Veuillez saisir 0 ou 1 : ";
        cin>>choix;
    }
    cout<<endl<<"Veuillez saisir le nom de l'image au format Mon_Image.png"<<endl;
    cout<<"Si le programme ne fonctionne pas, veuillez saisir le chemin d'accès entier de l'image"<<endl;
    cout<<"Saisie : ";
    cin>>img_path;
    if(choix == "0"){decode(img_path);}
    if(choix=="1"){encode(img_path);}
    return EXIT_SUCCESS;
}

unsigned char construction(unsigned char bleu, unsigned char vert, unsigned char rouge){
    unsigned char partie_bleu, partie_vert, partie_rouge, octet = 0;
    unsigned char masque_bleu = 0b00000011;
    unsigned char masque_vert_rouge = 0b00000111;
    partie_bleu = (bleu&masque_bleu)<<6;
    partie_vert = (vert&masque_vert_rouge)<<3;
    partie_rouge = rouge&masque_vert_rouge;
    octet = partie_bleu|partie_rouge|partie_vert;
    return octet;
}

int decode(string img_path){
    Mat img = imread(".\\images\\"+img_path, IMREAD_COLOR); //Chargement de l'image
    if(img.empty()){ //si elle ne charge pas message d'erreur
        system("cls");
        cout<<"impossible de charger l'image !"<<endl;
        cout<<"Veuillez appuyer sur une touche pour continuer...";
        _getch();
        system("cls");
        menu();
    }

    string message = ""; //déclaration de la string qui va accueillir le message caché

    for(int i = img.rows - 1; i>=0; i--){ //on parcour l'image ( a l'envers :') ) pour avoir le message qui s'affiche en premier
        for(int j = 0; j<img.cols; j++){
            Vec3b& couleur = img.at<Vec3b>(i, j);
            unsigned char octet = construction(couleur[0], couleur[1], couleur[2]);
            message += (char)octet;
            if(octet == 0){ //on trouve le \0 puis on arrête d'écrire dans la string puis on écrit la string dans un fichier texte
                system("cls");
                for(int k = 0; k<message.length()-1; k++){
                    cout<<message[k];
                }
                cout<<endl;
                cout<<endl<<"Veuillez appuyer sur une touche pour continuer...";
                _getch();
                menu();
            }
        }
    }
    return EXIT_SUCCESS;
}

int encode(string img_path){
    Mat img = imread(".\\images\\"+img_path, IMREAD_COLOR); //Chargement de l'image
    if(img.empty()){ //si elle ne charge pas message d'erreur
        system("cls");
        cout<<"impossible de charger l'image !"<<endl;
        cout<<"Veuillez appuyer sur une touche pour continuer...";
        _getch();
        system("cls");
        menu();
    }

    string message;
    system("cls");
    cout<<"Veuillez saisir le message à cacher dans l'image :"<<endl<<endl;
    cin.ignore();
    getline(cin, message);
    message+='\0';
    int compteur = 0;
    char masque_bleu = 0b11000000;
    char masque_vert = 0b00111000;
    char masque_rouge = 0b00000111;
    for(int i = img.rows - 1; i>=0; i--){ //on parcour l'image ( a l'envers :') ) pour avoir le message qui s'affiche en premier
        for(int j = 0; j<img.cols; j++){
            char partie_bleu = (message[compteur] & masque_bleu)>>6;
            char partie_vert = (message[compteur] & masque_vert)>>3;
            char partie_rouge = message[compteur] & masque_rouge;

            Vec3b& couleur = img.at<Vec3b>(i, j);
            couleur[0] = (couleur[0]&0b11111100)|partie_bleu;
            couleur[1] = (couleur[1]&0b11111000)|partie_vert;
            couleur[2] = (couleur[2]&0b11111000)|partie_rouge;
            if(compteur==message.size()-1){
                imwrite(".\\images\\"+img_path, img);
                cout<<endl<<"Le message est caché dans l'image."<<endl;
                cout<<endl<<"Veuillez appuyer sur une touche pour continuer...";
                _getch();
                menu();
            }
            compteur += 1;
        }
    }
    return EXIT_SUCCESS;
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    menu();
}