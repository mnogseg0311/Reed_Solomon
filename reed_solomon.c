//Marta Noguera Segura
//Eines informàtiques per a les matemàtiques
//Grau en Matemàtiques, UAB
//Curs 2021-22
//Treball final

#include "reed_solomon.h"

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

    if(ref==0){				//fitxer per configurar els valors de p i k
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
                else
                    fprintf(stderr, "\nADVERTENCIA: Hi ha un fitxer de configuracio amb parametres no valids, seguim amb els establerts per defecte");
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
        if(r==-1)                                              //si no es pot obrir el fitxer
            return 1;
        if(r%k==0)
            m=r/k;                                             //si no s'han d'afegir 0's
        else
            m=r/k+1;                                           //si s'han d'afegir 0's
     
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
        nomfinal[y-4]='\0';
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
           nomfinal[y-8]='\0';
        }
        else{
            strncpy(nomfinal, argv[argc-1], y-4);
            nomfinal[y-4]='\0';
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
