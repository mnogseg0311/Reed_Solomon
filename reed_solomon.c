//Marta Noguera Segura
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void configurapik(char* []);
int comprovapik(int, int);
int primer(int);
void llegeixcadena(char*, int*, int*, int*, int*, int*);
int decidirtipusfitxer(char*, int);
int contarvalors(int, int, char*);
int primitiu(int);
int ordre(int, int);
int potencia(int, int, int);
void llegeixmatambtipus(int, int d2, int, int, int, int, int[][d2], char*);
void llegeixmatdat(int d1, int d2, int p, int mat[][d2], char*, int);
void llegeixmatbin(int d1, int d2, int p, int mat[][d2], char*, int);
void llegeixmattxt(int d1, int d2, int p, int mat[][d2], char*, int);
void codificar(int, int, int, int, int[], int[]);
void descodificar(int, int, int, int, int[], int[], int*);
int discussio(int, int d2, int, int[], int[], int[][d2]);
int arreglarerrors(int, int, int, int, int, int[], int[]);
int rang(int, int d2, int, int[][d2]);
void gauss(int d1, int d2, int, int[][d2]);
void intercanviarfiles(int dim, int, int, int [][dim]);
void filaxescalar(int dim, int, int, int, int [][dim]);
void sumamultfila(int dim, int, int, int, int, int [][dim]);
int invers(int, int);

int main(int argc, char* argv[])
{
    if(argc<3){
        fprintf(stderr, "ERROR: Falten parametres\n");
        return 1;
    }
    if(argc>5){
        fprintf(stderr, "ERROR: Hi ha masses parametres\n");
        return 1;
    }

    char cadconf[10]="configura";
    if(argc==4 && strcmp(cadconf, argv[3])==0){                 //configurar fitxer amb p i k
        configurapik(argv);
        return 0;
    }

    int p=257;
    int k=200;
    int instruccio=0;
    int auxiliar=0;
    int ref=0;
    int* app=&p;
    int* apk=&k;
    int* api=&instruccio;
    int* apa=&auxiliar;
    int* apr=&ref;

    for(int i=1; i<argc-1; i++){                                //deixa el fitxer a part
        llegeixcadena(argv[i], app, apk, api, apa, apr);
    }

    if(auxiliar==1){
        fprintf(stderr, "ERROR: Els parametres introduits no son valids\n");
        return 1;
    }

    if(ref==0){
        FILE* fitxerconf;
        fitxerconf=fopen("RS-BW.cfg", "r");
        if(fitxerconf!=NULL){
                char cadenap[3]="p=";
                char cadenak[3]="k=";
                char auxp[10];
                fscanf(fitxerconf, "%s", auxp);
                if(strncmp(auxp, cadenap, 2)==0){
                    char valp[10];
                    strcpy(valp, auxp+2);
                    p=atoi(valp);
                }
                else{
                    fprintf(stderr, "\nADVERTENCIA: Hi ha un fitxer de configuracio amb parametres no valids, seguim amb els establerts per defecte");
                }
                char auxk[10];
                fscanf(fitxerconf, "%s", auxk);
                if(strncmp(auxk, cadenak, 2)==0){
                    char valk[10];
                    strcpy(valk, auxk+2);
                    k=atoi(valk);
                }
                else{
                    fprintf(stderr, "\nADVERTENCIA: Hi ha un fitxer de configuracio amb parametres no valids, seguim amb els establerts per defecte");
                }
                fclose(fitxerconf);
            }

    }

    if(comprovapik(p, k)==1){
        return 1;
    }

    int a=primitiu(p);
    int n=p-1;
    int m=0;

    int tipus=0;
    tipus=decidirtipusfitxer(argv[argc-1], instruccio);

    if(tipus==0 || tipus==-1){
        fprintf(stderr, "ERROR: Hi ha hagut un problema amb el fitxer\n");
        return 1;
    }

    if(tipus==3 && p<257){
        fprintf(stderr, "ERROR: Pels fitxers amb extensio txt nomes s'accepten primers majors o iguals a 257\n");
        return 1;
    }

    int y=strlen(argv[argc-1]);
    char nomfinal[30];

    if(instruccio==1){                                         //per codificar
        int r=contarvalors(instruccio, tipus, argv[argc-1]);
        if(r==-1){                                             //si no es pot obrir el fitxer
            return 1;
        }

        if(r%k==0){
            m=r/k;                                             //si no s'han d'afegir 0's
        }
        else{
            m=r/k+1;                                           //si s'han d'afegir 0's
        }

        int mat[m][k], mcod[m][n];                             //mat contindrà els vectors per codificar i mcod els codificats
        llegeixmatambtipus(m, k, r, p, tipus, instruccio, mat, argv[argc-1]);

        for(int i=0; i<m; i++){                                //cada fila de la matriu l'agafarem com un vector
            int v[k], vcod[n];                                 //v el vector a codificar, vcod, el codificat
            for(int j=0; j<k; j++){
                v[j]=mat[i][j];                                //estem agafant el vector
            }
            codificar(p, n, k, a, v, vcod);                    //el codifiquem

            for(int j=0; j<n; j++){
                mcod[i][j]=vcod[j];                            //posem el vector codificat a la matriu de vectors codificats
            }
        }

        strncpy(nomfinal, argv[argc-1], y-4);
        nomfinal[y-4]=0;
        strcat(nomfinal, ".RS");

        if(tipus==1){                                          //imprimir a fitxer amb extensió dat
            strcat(nomfinal, ".dat");

            FILE* fitxerfinal;
            fitxerfinal=fopen(nomfinal, "w");
            if(fitxerfinal==NULL){
                fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
                return 1;
            }
            for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){
                    fprintf(fitxerfinal, "%d \t", mcod[i][j]);
                }
            }
            fclose(fitxerfinal);
        }
        else{                                                   //imprimir a fitxer amb estensió bin
            if(tipus==2){
                strcat(nomfinal, ".bin");
            }
            else{
                strcat(nomfinal, ".txt.bin");
            }
            FILE* fitxerfinal;
            fitxerfinal=fopen(nomfinal, "wb");
            if(fitxerfinal==NULL){
                fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
                return 1;
            }
            for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){
                    int* aux=&mcod[i][j];
                    fwrite(aux, sizeof(int), 1, fitxerfinal);
                }
            }
            fclose(fitxerfinal);
        }
    }

    if(instruccio==2){                                          //per descodificar
        int s=contarvalors(instruccio, tipus, argv[argc-1]);
        if(s==-1){                                              //si no es pot obrir el fitxer
            return 1;
        }

        if(s%k==0){
            m=s/n;                                              //si no s'han d'afegir 0's
        }
        else{
            m=s/n+1;                                            //si s'han d'afegir 0's
        }

        int mcod[m][n], mat[m][k];                              //mcod contindrà els vectors per descodificar i mat els descodificats
        llegeixmatambtipus(m, n, s, p, tipus, instruccio, mcod, argv[argc-1]);

        for(int i=0; i<m; i++){                                 //cada fila de la matriu l'agafarem com un vector
            int vcod[n], v[k];                                  //vcod el vector a descodificar, v, el codificat
            for(int j=0; j<n; j++){
                vcod[j]=mcod[i][j];                             //estem agafant el vector
            }
            int aturar=0;
            int* apuntador=&aturar;
            descodificar(p, n, k, a, vcod, v, apuntador);       //el descodifiquem
            if(aturar==1){                                      //hi ha un error no corregible
                return 1;
            }
            for(int j=0; j<n; j++){
                mat[i][j]=v[j];                                 //posem el vector descodificat a la matriu de vectors descodificats
            }
        }

        if(tipus==3){
           strncpy(nomfinal, argv[argc-1], y-8);
           nomfinal[y-8]=0;
        }
        else{
            strncpy(nomfinal, argv[argc-1], y-4);
            nomfinal[y-4]=0;
        }
        strcat(nomfinal, ".BW");

        if(tipus==1){                                           //imprimir a fitxer amb extensió dat
            strcat(nomfinal, ".dat");

            FILE* fitxerfinal;
            fitxerfinal=fopen(nomfinal, "w");
            if(fitxerfinal==NULL){
                fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
                return 1;
            }
            for(int i=0; i<m; i++){
                for(int j=0; j<k; j++){
                    fprintf(fitxerfinal, "%d \t", mat[i][j]);
                }
            }
            fclose(fitxerfinal);
        }
        if(tipus==2){                                           //imprimir a fitxer amb estensió bin
            strcat(nomfinal, ".bin");

            FILE* fitxerfinal;
            fitxerfinal=fopen(nomfinal, "wb");
            if(fitxerfinal==NULL){
                fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
                return 1;
            }
            for(int i=0; i<m; i++){
                for(int j=0; j<k; j++){
                    int* aux=&mat[i][j];
                    fwrite(aux, sizeof(int), 1, fitxerfinal);
                }
            }
            fclose(fitxerfinal);
        }
        if(tipus==3){                                           //imprimir a fitxer amb extensió txt
            strcat(nomfinal, ".txt");

            FILE* fitxerfinal;
            fitxerfinal=fopen(nomfinal, "w");
            if(fitxerfinal==NULL){
                fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
                return 1;
            }
            for(int i=0; i<m; i++){
                for(int j=0; j<k; j++){
                    char aux[10];
                    sprintf(aux, "%d", mat[i][j]);
                    fprintf(fitxerfinal, "%s \t", aux);
                }
            }
            fclose(fitxerfinal);
        }
    }

    return 0;
}

void configurapik(char* argv[])
{
    char cadguio[2]="-";
    char cadp[4]="-p=";
    char cadk[4]="-k=";
    int p=0, k=0;

    for(int i=1; i<=2; i++){
            if(strncmp(argv[i], cadguio, 1)==0){                //si comencen amb -
                char valor[10];
                if(strncmp(argv[i], cadp, 3)==0){               //per donar valor a la p
                    strcpy(valor, argv[i]+3);
                    p=atoi(valor);
                }
                if(strncmp(argv[i], cadk, 3)==0){               //per donar valor a la k
                    strcpy(valor, argv[i]+3);
                    k=atoi(valor);
                }
            }
            else{
                fprintf(stderr, "ERROR: Parametre no valid\n");
                return;
            }
        }

        if(comprovapik(p, k)==1){
            return;
        }

        FILE * fitxerconfigurar;
        fitxerconfigurar=fopen("RS-BW.cfg", "w");               //creem el fitxer, si ja hi era, l'anterior es perd
        if(fitxerconfigurar==NULL){
            fprintf(stderr, "ERROR: El fitxer de configuracio no es pot obrir\n");
            return;
        }
        fprintf(fitxerconfigurar, "p=%d\n", p);                 //hi escrivim p i k
        fprintf(fitxerconfigurar, "k=%d", k);
        fclose(fitxerconfigurar);
        fprintf(stderr, "S'ha creat el fitxer de configuracio pels valors de p i k introduits\n");
        return;
}

int comprovapik(int p, int k)
{
    if(primer(p)==1){
        fprintf(stderr, "ERROR: p ha de ser primer\n");
        return 1;
    }
    if(p<5){
        fprintf(stderr, "ERROR: p ha de ser major o igual a 5\n");
        return 1;
    }
    if(k>=p){
        fprintf(stderr, "ADVERTENCIA: k major que p, n'utilitzem la seva reduccio a modul p\n");
        k=k%p;
    }
    if(p<k+3){
        fprintf(stderr, "ERROR: p ha de ser major o igual a k+3\n");
        return 1;
    }
    if(k<=1){
        fprintf(stderr, "ERROR: k ha de ser major a 1\n");
        return 1;
    }
    return 0;
}

int primer(int n)
{
    int prod=1;
    for(int i=2; i<=n/2; i++){
        if(n%i==0){
            prod=i*prod;
        }
    }
    if(prod==1 && n>1){
        return 0;                                                //si és primer
    }
    else {
        return 1;                                                //si no és primer
    }
}

void llegeixcadena(char* allegir, int* p, int* k, int* instr, int* aux, int* ref)
{
    int aux2=0;                                                 //per donar error si hi ha un parametre diferent dels esperats
    char cadguio[2]="-";
    char cadp[4]="-p=";
    char cadk[4]="-k=";
    char cadcod[9]="codifica";
    char caddescod[12]="descodifica";

    if(strncmp(allegir, cadguio, 1)==0){                        //si comencen amb -
        char valor[10];
        if(strncmp(allegir, cadp, 3)==0){                       //per donar valor a la p
            if(*p==257){
                strcpy(valor, allegir+3);
                *p=atoi(valor);
                *ref=1;                                         //per saber si hem donat valors
                aux2++;
            }
            else{                                               //no s'admeten dos valors per p
                *aux=1;
                return;
            }
        }
        if(strncmp(allegir, cadk, 3)==0){                       //per donar valor a la k
            if(*k==200){
                strcpy(valor, allegir+3);
                *k=atoi(valor);
                *ref=1;
                aux2++;
            }
            else{                                               //no s'admeten dos valors per k
                *aux=1;
                return;
            }
        }
    }

    if(strcmp(cadcod, allegir)==0){
        if(*instr==0){
            *instr=1;                                           //s'ha de codificar
            aux2++;
        }
        else{                                                   //no s'admeten dos instruccions
            *aux=1;
            return;
        }
    }

    if(strcmp(caddescod, allegir)==0){
        if(*instr==0){
            *instr=2;                                           //s'ha de descodificar
            aux2++;
        }
        else{                                                   //no s'admeten dos instruccions
            *aux=1;
            return;
        }
    }

    if(aux2==0){                                                //hi ha algun parametre que no coincideix amb cap dels esperats
        *aux=1;
    }
    return;
}

int decidirtipusfitxer(char* nomfitxer, int instr)
{
    int x=strlen(nomfitxer);
    char cdat[5]=".dat";
    char cbin[5]=".bin";
    char ctxt[5]=".txt";
    char ctxtbin[9]=".txt.bin";
    char terminacio[9];
    char terminacio2[9];

    if(instr==1){                                               //per codificar
        strcpy(terminacio, nomfitxer+x-4);
        if(strcmp(cdat, terminacio)==0){
            return 1;
        }
        if(strcmp(cbin, terminacio)==0){
            return 2;
        }
        if(strcmp(ctxt, terminacio)==0){
            return 3;
        }
    }

    if(instr==2){                                               //per descodificar
        strcpy(terminacio, nomfitxer+x-4);
        strcpy(terminacio2, nomfitxer+x-8);

        if(strcmp(cdat, terminacio)==0){
            return 1;
        }
        if(strcmp(cbin, terminacio)==0){
            if(strcmp(ctxtbin, terminacio2)==0){
                return 3;
            }
            else{
                return 2;
            }
        }
    }
    return -1;
}

int contarvalors(int instr, int tipus, char* nomfitxer)
{
    FILE* nansafitxer;

    if(tipus==2 || (tipus==3 && instr==2)){                     //si el fitxer és .bin
        nansafitxer=fopen(nomfitxer, "rb");
    }
    else{
        nansafitxer=fopen(nomfitxer, "r");                      //si el fitxer és .dat o .txt
    }

    if(nansafitxer == NULL){
        fprintf(stderr, "ERROR: El fitxer no es pot obrir\n");
        return -1;
    }

    int num=0;
    if(tipus==1){                                               //contar els valors d'un fitxer dat
        int val;
        while(fscanf(nansafitxer, "%d", &val)!=EOF){
            num++;
        }
        fclose(nansafitxer);
    }
    if(tipus==2 || (tipus==3 && instr==2)){                     //contar els valors d'un fitxer bin
        int aux=0;

        for(int i=0; i>=0; i++){
            fread(&aux, sizeof(int), 1, nansafitxer);
            if(feof(nansafitxer)){
                break;
            }
            aux=0;
            num++;
        }
        fclose(nansafitxer);
    }
    if(tipus==3 || instr==2){                                   //contar els valors d'un fitxer txt
        char val[10];
        while(fscanf(nansafitxer, "%s", val)!=EOF){
            num++;
        }
        fclose(nansafitxer);
    }
    return num;
}

int primitiu(int p)
{
    int i=1;
    while(ordre(i, p)!= p-1){
        i++;
    }
    return i;
}

int ordre(int a, int p)
{
    int n=1, b=a;
    while(b%p!=1){
        b=b*a;
        b=b%p;
        n++;
    }
    return n;
}

int potencia(int x, int n, int p)
{
    int pot=1;
    for(int i=1; i<=n; i++){
        pot=pot*x;
        pot=pot%p;
    }
    return pot;
}

void llegeixmatambtipus(int d1, int d2, int h, int p, int tipus, int instr, int mat[][d2], char* fitxer)
{
    int res=h%d2;
    if(tipus==1){                                               //llegir els valors d'un fitxer dat
        llegeixmatdat(d1, d2, p, mat, fitxer, res);
        return;
    }

    if(tipus==2 || (tipus==3 && instr==2)){                     //llegir els valors d'un fitxer bin
        llegeixmatbin(d1, d2, p, mat, fitxer, res);
        return;
    }
    if(tipus==3 || instr==2){                                   //llegir els valors d'un fitxer txt
        llegeixmattxt(d1, d2, p, mat, fitxer, res);
        return;
    }
}

void llegeixmatdat(int d1, int d2, int p, int m[][d2], char* fitxer, int res)
{
    FILE* nansafitxer;
    nansafitxer=fopen(fitxer, "r");                             //si no es pot obrir ja haurà donat error abans

    int daux=0;
    if(res==0){
        daux=d1;
    }
    else{
        daux=d1-1;
    }
    for(int i=0; i<daux; i++){
        for(int j=0; j<d2; j++){
            int aux;
            fscanf(nansafitxer, "%d", &aux);
            if(aux<0 || aux>=p){
                aux=aux%p;
                if(aux<0){
                    aux=aux+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[i][j]=aux;
        }
    }
    if(res!=0){
        for(int j=0; j<res; j++){                               //llegim els valors de l'últim vector
            int aux;
            fscanf(nansafitxer, "%d", &aux);
            if(aux<0 || aux>=p){
                aux=aux%p;
                if(aux<0){
                    aux=aux+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[daux][j]=aux;
        }
        for(int j=res; j<d2; j++){                              //omplim l'últim vector amb 0's
            m[daux][j]=0;
        }
    }
    fclose(nansafitxer);
    return;
}

void llegeixmatbin(int d1, int d2, int p, int m[][d2], char* fitxer, int res)
{
    FILE* nansafitxer;
    nansafitxer=fopen(fitxer, "rb");                            //si no es pot obrir ja haurà donat error abans

    int daux=0;
    if(res==0){
        daux=d1;
    }
    else{
        daux=d1-1;
    }
    for(int i=0; i<daux; i++){
        for(int j=0; j<d2; j++){
            int aux;
            fread(&aux, sizeof(int), 1, nansafitxer);
            if(aux<0 || aux>=p){
                aux=aux%p;
                if(aux<0){
                    aux=aux+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[i][j]=aux;
        }
    }
    if(res!=0){
        for(int j=0; j<res; j++){                               //llegim els valors de l'últim vector
            int aux;
            fread(&aux, sizeof(int), 1, nansafitxer);
            if(aux<0 || aux>=p){
                aux=aux%p;
                if(aux<0){
                    aux=aux+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[daux][j]=aux;
        }
        for(int j=res; j<d2; j++){                              //omplim l'últim vector amb 0's
            m[daux][j]=0;
        }
    }
    fclose(nansafitxer);
    return;
}

void llegeixmattxt(int d1, int d2, int p, int m[][d2], char* fitxer, int res)
{
    FILE* nansafitxer;
    nansafitxer=fopen(fitxer, "r");                             //si no es pot obrir ja haurà donat error abans

    int daux=0;
    if(res==0){
        daux=d1;
    }
    else{
        daux=d1-1;
    }
    for(int i=0; i<daux; i++){
        for(int j=0; j<d2; j++){
            char aux1[10];
            int aux2;
            fscanf(nansafitxer, "%s", aux1);
            aux2=atoi(aux1);
            if(aux2<0 || aux2>=p){
                aux2=aux2%p;
                if(aux2<0){
                    aux2=aux2+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[i][j]=aux2;
        }
    }
    if(res!=0){
        for(int j=0; j<res; j++){                               //llegim els valors de l'últim vector
            char aux1[10];
            int aux2;
            fscanf(nansafitxer, "%s", aux1);
            aux2=atoi(aux1);
            if(aux2<0 || aux2>=p){
                aux2=aux2%p;
                if(aux2<0){
                    aux2=aux2+p;
                }
                fprintf(stderr, "\nADVERTENCIA: Valor fora del cos, n'utilitzem la seva reduccio a modul\n");
            }
            m[daux][j]=aux2;
        }
        for(int j=res; j<d2; j++){                              //omplim l'últim vector amb 0's
            m[daux][j]=0;
        }
    }
    fclose(nansafitxer);
    return;
}

void codificar(int p, int n, int k, int a, int v[], int vcod[])
{

    int RS[n][k];
    for(int i=0; i<n; i++){
        for(int j=0; j<k; j++){
            RS[i][j]=potencia(a, i*j, p);
        }
    }

    for(int i=0; i<=n; i++){
        int sum=0;
        for(int j=0; j<k; j++){
            sum=sum + RS[i][j]*v[j];
            sum=sum%p;
        }
        vcod[i]=sum;
    }
    return;
}

void descodificar(int p, int n, int k, int a, int vcod[], int v[], int* apuntador)
{
    int RS[n][k];
    for(int i=0; i<n; i++){
        for(int j=0; j<k; j++){
            RS[i][j]=potencia(a, i*j, p);
        }
    }

    int x=discussio(n, k, p, vcod, v, RS);
    if(x==1){                                                   //si és un SCD, el vector v ja tindrà la solució
        return;
    }
    else {
        int t=n-k;
        int e=t/2;
        while(e>=1){
            int d=arreglarerrors(p, n, k, a, e, vcod, v);       //si d=0, SCI i segueix el bucle
            if(d==-1){
                fprintf(stderr, "\nError no corregible\n");
                *apuntador=1;                                   //per aturar el programa
                return;
            }
            if(d==1){
                return;                                         //el vector v ja tindrà la solució
            }
            e=e-1;
        }
    }
}

int discussio(int d1, int d2, int p, int v[], int u[], int m[][d2])
{
    int ampliada[d1][d2+1];                                     //declarem la matriu ampliada associada al sistema
    for(int i=0; i<d1; i++){
        for(int j=0; j<d2; j++){
            ampliada[i][j]=m[i][j];                             //els coeficients de la matriu
        }
    }
    for(int i=0; i<d1; i++){
        ampliada[i][d2]=v[i];                                   //els coeficients del vector resultat del sistema
    }
    int a=rang(d1, d2, p, m);
    int b=rang(d1, d2+1, p, ampliada);

    if(a==b){
        if(a==d2){                                              //SCD, d2 és el nombre d'incògnites
            gauss(d1, d2+1, p, ampliada);

            for(int i=0; i<d1; i++){                            //u és el vector solució del sistema (SCD)
                u[i]=ampliada[i][d2];
            }
            return 1;
        }
        if(a!=d2){
            return 0;                                           //SCI
        }
    }
    if(a!=b){
        return -1;                                              //SI
    }
    return 2;
}

int arreglarerrors(int p, int n, int k, int a, int e, int vcod[], int v[])
{
    int BW[n][2*e+k];
    for(int i=0; i<n; i++){
        for(int j=0; j<e+k; j++){                               //la primera part amb les potències de a
            BW[i][j]=potencia(a, i*j, p);
        }
    }
    for(int i=0; i<n; i++){
        int l=0;
        for(int j=e+k; j<2*e+k; j++){                           //la segona part amb les potències de a per els valors a descodificar en negatiu
            BW[i][j]=-1*vcod[i]*potencia(a, i*l, p);
            BW[i][j]=BW[i][j]%p;                                //reduïm a mòdul
            if(BW[i][j]<0){
                BW[i][j]=BW[i][j]+p;
            }
            l++;
        }
    }

    int vres[n];
    int vaux[2*e+k];

    for(int i=0; i<n; i++){                                     //el resultant del sistema auxiliar amb BW
        vres[i]=vcod[i]*potencia(a, i, p);
    }

    int d=discussio(n, 2*e+k, p, vres, vaux, BW);

    if(d==1){                                                   //vaux tindrà les solucions del sistema auxiliar
        int M3[e+k][k];
        int g[e+k];

        for(int j=0; j<k; j++){
            int l=e+k;
            for(int i=j; i<j+e; i++){                           //posa els valors de f sub i guardats a vaux
                M3[i][j]=vaux[l];
                l++;
            }
            M3[j+e][j]=1;                                       //posa els 1's sots de f sub e
        }
        for(int j=0; j<k; j++){
            for(int i=0; i<j; i++){                             //posa els 0's per sobre de la diagonal
                M3[i][j]=0;
            }
            for(int i=j+e+1; i<e+k; i++){                       //posa la resta de 0's a sota
                M3[i][j]=0;
            }
        }

        for(int i=0; i<e+k; i++){                               //posa els valors de g sub i guardats a vaux
            g[i]=vaux[i];
        }

        int z=discussio(e+k, k, p, g, v, M3);

        return z;
    }
    else {
        return d;
    }
}

int rang(int d1, int d2, int p, int m[][d2])
{
    gauss(d1, d2, p, m);
    int aux=0;                                                  //per cada fila mirem si és no nul·la i acomulem el nombre no nul·les a aux
    for(int i=0; i<d1; i++){
        for(int j=0; j<d2; j++){
            if(m[i][j]!=0){
                aux++;
                break;                                          //en trobar un coeficient diferent de 0, passem a la següent fila
            }
        }
    }
    return aux;
}

void gauss(int d1, int d2, int p, int m[][d2])
{
    int a=0;                                                    //a marca a quina fila li estem buscan pivot
    for(int j=0; j<d2; j++){
        if(a>=d1){
            break;
        }
        int i=a;
        while(m[i][j]==0 && i<d1){                              //busquem una fila on el coeficient sigui diferent de 0
            i++;
        }
        if(i<d1){                                               //si totes són 0 no farà res
            if(i!=a){                                           //pujem la fila, si cal
                intercanviarfiles(d2, a, i, m);
            }
            if(m[a][j]!=1){                                     //obtenim el pivot 1
                filaxescalar(d2, p, invers(m[a][j], p), a, m);
            }
            for(int k=a+1; k<d1; k++){                          //fem 0's
                if(m[k][j]!=0){
                    sumamultfila(d2, p, (-1*m[k][j]), k, a, m);
                }
            }
            a++;                                                //només passa a la següent fila si hi ha pivot,
        }
    }                                                           //tenim pivots 1's i 0's a sota de la diagonal
                                                                //volem fer 0's per sobre de la diagonal
    for(int i=d1-1; i>0; i=i-1){                                //ho fem per cada fila
        int b=0;
        for(int j=0; j<d2; j++){
            if(m[i][j]==0){
                b++;
            }
            else{
                break;                                          //si n'hi ha, b marca la columna de l'últim pivot
            }
        }
        if(b<d2){                                               //si no es una fila de 0's
            for(int s=i-1; s>=0; s=s-1){                        //des de la fila del damunt a la primera
                if(m[s][b]!=0){
                    sumamultfila(d2, p, (-1*m[s][b]), s, i, m); //fem 0's
                }
            }
        }
    }
    return;
}

void intercanviarfiles(int dim, int l, int k, int m[][dim])
{
    for(int j=0; j<dim; j++){
        int aux=m[l][j];
        m[l][j]=m[k][j];
        m[k][j]=aux;
    }
    return;
}

void filaxescalar(int dim, int p, int n, int k, int m[][dim])
{
    for(int j=0; j<dim; j++){
        m[k][j]=n*m[k][j];
        m[k][j]=m[k][j]%p;
        while(m[k][j]<0){
            m[k][j]=m[k][j]+p;
        }
    }
    return;
}

void sumamultfila(int dim, int p, int n, int k, int l, int m[][dim])
{
    for(int j=0; j<dim; j++){
        m[k][j]=m[k][j]+n*m[l][j];
        m[k][j]=m[k][j]%p;
        while(m[k][j]<0){
            m[k][j]=m[k][j]+p;
        }
    }
    return;
}

int invers(int a, int p)                                        //Bézout i Euclides
{
    int p0=p;
    int r=p%a, s=-1*(p/a);
    int x1=1, x2, y=s;
    while(r>1){
        p=a;
        a=r;
        r=p%a;
        x2=s;
        s=x1-(p/a)*y;
        x1=x2;
        y=s;
    }
    if(s<0){
        s+=p0;
    }
    return s;
}
