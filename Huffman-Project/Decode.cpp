#include "Decode.h"


    QByteArray FromHead(char c1, char c2){
        QByteArray code;
        code.append(set(QByteArray::number((unsigned char)c1,2),8));
        code.append(set(QByteArray::number((unsigned char)c2,2),8));
        return code;
    }

       
    QPair<int, int> Sizes(QByteArray code){
        bool ok;
        QByteArray y = code.left(3), z = code.right(13);


        int trash = y.toInt(&ok,2) ;
        int treeSize =z.toInt(&ok,2);
        return QPair<int,int> (trash, treeSize);
    }




     QPair<int, QString> unzip(QString name){

         if(name.right(5) != (QString)".huff"){
               return QPair<int, QString> (2,"");
        }

         QFile newfile(name);
           if(!newfile.open(QIODevice::ReadOnly)) {
               return QPair<int, QString> (1,"");
            }

     // Lendo o cabeçalho:
           QByteArray aux = newfile.read(3);
            int tamName = aux.at(2);
            aux = FromHead(aux[0],aux[1]);

           int trash = Sizes(aux).first ;
           int tamArv = Sizes(aux).second;


     // Recebe o nome do arquivo original:
           QByteArray original = newfile.read(tamName);


     // Recebe a representação da árvore:
            QByteArray T;
            T.append(newfile.read(tamArv));


           Node *tree;
           tree = tree->FromByteArray(T, 0).first;

     // Converte os caracteres do arquivo em binário:
          QByteArray aux2;
          QByteArray text;
          Node* current = tree;
          QFile myfile(original);

          if(!myfile.open(QIODevice::WriteOnly)){
             return QPair<int, QString> (1,"");
          }

           while (!newfile.atEnd()) {
               QByteArray line = newfile.read(1024);
               for(int i=0; i<line.size(); ++i){
                  unsigned char currentch = line.at(i);
                    QByteArray a = set(QByteArray::number(currentch,2),8);

                    aux2.append(a);
               }

        //   Remove o lixo:
                      if(newfile.atEnd()) aux2.chop(trash);

        //  Reconstrói o arquivo original:
                      for(int i=0; i< aux2.size(); ++i){
                         if(aux2.at(i) == '0'){
                             current = current->left;
                         }
                         else{
                             current = current->right;
                         }
                         if(current->isLeaf()){
                             unsigned char c = current->content;
                             text.append(c);
                             current = tree;
                         }
                      }
                      
                myfile.write(text);
                text.clear();
                aux2.clear();
           }

           myfile.close();
           return QPair<int, QString> (0,original);

     }
