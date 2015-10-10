#include "serveur.h"
#include "ui_serveur.h"
#define JOUR_ESSAI 30
#define NBRE_TENTATIVE 3

Serveur::Serveur(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::Serveur)
{
    m_ui->setupUi(this);

    serveur = new QTcpServer(this); serveur->setMaxPendingConnections(40);
    etatServeur=new QLabel(m_ui->sMenu); nbreConnectes=new QLabel(m_ui->etat);
    etatServeur->setStyleSheet("color: rgb(0, 200, 0);"); nbreConnectes->setStyleSheet("color: rgb(255, 255, 255);");
    m_ui->connect->setStyleSheet("background-color: rgb(40, 40, 40); color: rgb(255, 255, 255); border-radius: 12px;");
    m_ui->rap->setStyleSheet("background-color: rgb(40, 40, 40); color: rgb(255, 255, 255); border-radius: 12px;");
    etatServeur->setStyleSheet("font: 10pt ;color: rgb(255, 255, 255);");

    nbreConnectes->setFixedSize(300, 23); etatServeur->setFixedSize(500, 23);
  
    ETAT="STOP"; ADMIN_CONNECTED=false;
    modele= new QStandardItemModel(0,0);
    construireVue(); m_ui->boiteRapport->hide(); openConfig(); initialiserCryptage();


    connect(m_ui->dossier, SIGNAL(clicked(QModelIndex)),this,SLOT(selection()));
}

void Serveur::initialiserCryptage()
{
   crypt<< "A"; crypt<< "B"; crypt<< "C"; crypt<< "D"; crypt<< "E"; crypt<< "F"; crypt<< "G"; crypt<< "H"; crypt<< "I"; crypt<< "J";
  crypt<< "K"; crypt<< "L"; crypt<< "M"; crypt<< "N"; crypt<< "O"; crypt<< "P"; crypt<< "Q"; crypt<< "R"; crypt<< "S"; crypt<< "T";
  crypt<< "U"; crypt<< "V"; crypt<< "W"; crypt<< "X"; crypt<< "Y"; crypt<< "Z";

 cryptLower<< "a";  cryptLower<< "b"; cryptLower<< "c"; cryptLower<< "d"; cryptLower<< "e"; cryptLower<< "f"; cryptLower<< "g";
  cryptLower<< "h"; cryptLower<< "i"; cryptLower<< "j"; cryptLower<< "k"; cryptLower<< "l"; cryptLower<< "m"; cryptLower<< "n";
   cryptLower<< "o"; cryptLower<< "p"; cryptLower<< "q"; cryptLower<< "r"; cryptLower<< "s"; cryptLower<< "t"; cryptLower<< "u";
    cryptLower<< "v"; cryptLower<< "w"; cryptLower<< "x"; cryptLower<< "y"; cryptLower<< "z";

  cryptNombre<< "0"; cryptNombre<< "1";  cryptNombre<< "2";  cryptNombre<< "3";  cryptNombre<< "4";  cryptNombre<< "5";  cryptNombre<< "6";
  cryptNombre<< "7";  cryptNombre<< "8";  cryptNombre<< "9";
}

void Serveur::closeEvent(QCloseEvent *event)
{m_ui->rap->click(); saveConfig(); m_ui->boiteRapport->close();}

void Serveur::saveConfig()
{
 QFile f("CONF.vix");
 f.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text); QTextStream flux(&f);
 if(f.isOpen()) flux<<m_ui->port->text();
 f.close();
}

void Serveur::openConfig()
{
 QFile f("CONF.vix"); QString string;
 f.open(QIODevice::ReadOnly);
 if(f.isOpen()){string=f.readLine(100);m_ui->port->setText(string.section("\r\n",0,0));}
 else m_ui->port->setText("50885");
 f.close();
}

void Serveur::ouvertureServeur()
{
    if (!serveur->listen(QHostAddress::Any, m_ui->port->text().toInt())) // Démarrage du serveur sur toutes les IP disponibles et sur le port PORT
    {
        // Si le serveur n'a pas été démarré correctement
        etatServeur->setText(tr(" Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());
    }
    else
    {
        // Si le serveur a été démarré correctement
        etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    tailleMessage = 0;
}

void Serveur::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));

 nbreConnectes->setText(QString("Nombre de connectés: %1").arg(clients.size()));
}

void Serveur::donneesRecues()
{
    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;
    // Si tout va bien, on continue : on récupère le message
    QDataStream in(socket);
    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
             return;
        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }
    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
        return;
    // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
    QByteArray passe, code, ID;
    QString codeBancaire,nom, cK;
    int periodeActivation;
    in >>LOGICIEL;
    in >> CLEF;
    in >> ID;
    /*On récupère la session ouverte*/
    in >>nom;
    in >>passe;
    /*On récupère la demande d'inscription*/
    in >>uneSession.nom;
    in >>uneSession.passe;
    in >>uneSession.number;
    in >>code;
    in >>codeActualiser;
    in >>periodeActivation;
    in >>codeBancaire;

    /////////////////POUR ADMINISTRATION/////////////////////
    in >>CLEF_ADMIN;
    in >>cK;

    textePlus="";// texte de rapport pour Administrateur
    if(CLEF_ADMIN=="SAVE")m_ui->rap->click();

        //////////////////POUR ADMINISTRATEUR/////////
    QString all="ADMIN"; QString stringAll=QCryptographicHash::hash(all.toUtf8(), QCryptographicHash::Sha1).toHex();
    if(LOGICIEL==stringAll)fonctionAdministrateur();
    else
    {
     all="USER"; stringAll=QCryptographicHash::hash(all.toUtf8(), QCryptographicHash::Sha1).toHex();
     if(LOGICIEL==stringAll){ADMIN_CONNECTED=false;}
    }

 if(cK!="")
    {if(cK.section("=",3,3)=="SUP")supprimerClef(cK); else creerClef(cK);}
 else{///////SI ON N'UTILISE PAS LE GENERATEUR DE CLEE
    uneSession.Id=ID; if(uneSession.nom=="Pseudo Activix" | uneSession.nom=="") uneSession.nom=nom;
    if(uneSession.passe=="" | uneSession.passe=="Mot de passe") uneSession.passe=passe;
    if(ID=="")
    {m_ui->rapport->append(QString("<strong> ATTENTION: L'identifiant de %1 vaut NULL</strong>   <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom));
     textePlus+=QString("<strong> ATTENTION: L'identifiant de %1 vaut NULL</strong>   <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom);
    }
        //LES ACTIONS SUIVANTES////////////////////

    creerRepertoires(LOGICIEL);

    if(CLEF=="INSCRIRE") creationDeCompte(uneSession);
    if(CLEF=="CONNECTER")ouvertureSession(nom, passe);
    if(CLEF=="DECONNECTER")
    {CLEF_RETOUR="DECONNECTER";
     m_ui->rapport->append("Fermeture de la session <strong>" + nom + " </strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
     textePlus+="Fermeture de la session <strong>" + nom + " </strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
    }
    if(CLEF=="CONFIRMER_COMPTE")confirmerCompte(uneSession,code);
    if(CLEF=="ACTIVATION")activationDuLogiciel(periodeActivation, uneSession.nom);
    if(CLEF=="TRANSFERT") activation(transfertBancaire(codeBancaire+"10S395S",uneSession.nom));
    if(CLEF=="ESSAI")dossierDEssaiGratuit(ID);
    if(CLEF=="ACTUALISER_ACCES") actualiserAcces();
  }
    if(ADMIN_CONNECTED && socketAdmin!=0 && socketAdmin->isValid())envoyerDonneesAdmin();
    // remise de la taille du message à 0 pour permettre la réception des futurs messages
    tailleMessage = 0;
}

void Serveur::supprimerClef(QString key)
{
  QString nomDoc="CleE_kYEs";
  QFile f("ACTVIX/"+key.section("=",2,2)+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()
          +"/"+key.section("=",0,0)+".vix");

  if(f.exists()) {f.remove();  CLEF_ADMIN="KEY_SUP";} else CLEF_ADMIN="KEY_SUP";
}

void Serveur::creerClef(QString key)
{
 creerRepertoires(key.section("=",2,2));
 QString nomDoc="CleE_kYEs"; QStringList liste;
  QFile f("ACTVIX/"+key.section("=",2,2)+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()
          +"/"+key.section("=",0,0)+".vix");
   QTextStream flux(&f);
  if(!f.exists()){
   f.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
   flux<<key.section("=",1,1);
   f.close();

   QDir dir("ACTVIX/"+key.section("=",2,2)+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex());
   liste=dir.entryList(QDir::Files);

   CLEF_ADMIN="KEY_CREE "+key.section("=",2,2).section("_",0,0)+" ";
 }
  else CLEF_ADMIN="KEY_ECHEC "+key.section("=",2,2).section("_",0,0)+" ";

  for(int i=0;i<liste.size();i++) CLEF_ADMIN+=liste[i].section(".",0,0);
}

void Serveur::envoyerDonnees()
{
     // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket ==0) return;

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << CLEF_RETOUR; // On ajoute le message à la suite
    out << texteActivation;
    out <<Njour;
    out <<jourRestant;
    out <<AUTORISATION;
    out <<codeG;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message
    // Envoi du paquet préparé

   if(socket->isValid())socket->write(paquet); 

}

void Serveur::fonctionAdministrateur()
{
 socketAdmin = qobject_cast<QTcpSocket *>(sender()); ADMIN_CONNECTED=true;
}

void Serveur::envoyerDonneesAdmin()
{
      // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QString clee="ADMINISTRATEUR";
    if(CLEF_ADMIN=="SHOPPER")textePlus=m_ui->rapport->toHtml();
    if(CLEF_ADMIN.section(" ",0,0)=="KEY_CREE" | CLEF_ADMIN.section(" ",0,0)=="KEY_ECHEC")textePlus=CLEF_ADMIN;
    if(CLEF_ADMIN=="KEY_SUP")textePlus=CLEF_ADMIN;

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << clee; // On ajoute le message à la suite
    out << texteActivation;
    out <<Njour;
    out <<jourRestant;
    out <<AUTORISATION;
    out <<codeG;

    out <<textePlus;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message
    // Envoi du paquet préparé

   socketAdmin->write(paquet);
}

void Serveur::creerRepertoires(QString rep)
{
  QDir dossier; QString nomDoc="CleE_kYEs";
  if(!dossier.exists("ACTVIX")) dossier.mkdir("ACTVIX");
  if(!dossier.exists("ACTVIX/"+rep)) dossier.mkdir("ACTVIX/"+rep);
  if(!dossier.exists("ACTVIX/"+rep+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()))
      dossier.mkdir("ACTVIX/"+rep+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex());

 m_ui->rapport->append(QString("<strong>*************************%1***********************</strong>").arg(LOGICIEL.section("_",0,0)));
 m_ui->rapport->ensureCursorVisible();
 textePlus+=QString("<strong>*************************%1***********************</strong>").arg(LOGICIEL.section("_",0,0));
}

void Serveur::dossierDEssaiGratuit(QString Id)
{
  QDir dossier; QString nomDoc="ESSAI GRATUIT";
 if(Id!="")
 {
  if(!dossier.exists("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()))
      dossier.mkdir("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex());
 QFile file("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()+"/"+Id+".vix"); QTextStream flux(&file);
 if(!file.exists())
 {
  file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
  flux<<QDate::currentDate().toString()<<endl; flux<<"1";

  m_ui->rapport->append(QString("Activation d'1e période d'essai par <strong>%1</strong> : %2    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom).arg(Id));
  m_ui->rapport->ensureCursorVisible();
  textePlus+=QString("Activation d'1e période d'essai par <strong>%1</strong> : %2    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom).arg(Id);

  CLEF_RETOUR="ESSAI_OK"; envoyerDonnees();

  construireVue();
 }

 else
 {CLEF_RETOUR="ESSAI_UNOK";
  m_ui->rapport->append(QString("Période d'essai réfusée à <strong>%1</strong> : %2    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom).arg(Id));
  m_ui->rapport->ensureCursorVisible();
  textePlus+=QString("Période d'essai réfusée à <strong>%1</strong> : %2    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(uneSession.nom).arg(Id);
 }
}
}

QString Serveur::suspensionDeLicence(QString Id)
{
   QString nomDoc="ESSAI GRATUIT"; int Nj_essai=0, Nj_abon=0;
  QFile file("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()+"/"+Id+".vix");
  if(file.exists())
  {file.open(QIODevice::ReadOnly); QString ligne=file.readLine(100); file.close();
   if(ligne.section("\r\n",0,0)!=""){
   QDate d=QDate::fromString(ligne.section("\r\n",0,0));
   Nj_essai=d.daysTo(QDate::currentDate());}
  }

  QFile f("ACTVIX/"+LOGICIEL+"/COMPS/"+uneSession.nom+"@"+uneSession.passe+"/"+Id+".vix"); QStringList ligne; int Nj_souscrit=0;
  QDate d;
 if(f.exists())
 {f.open(QIODevice::ReadOnly);
  while(!f.atEnd()) ligne<<f.readLine(100); f.close();
  if(2<=ligne.size())
  {d=QDate::fromString(ligne[1].section("\r\n",0,0)); Nj_souscrit=ligne[0].section("\r\n",0,0).toInt();
   Nj_abon= d.daysTo(QDate::currentDate());
  }
 }

 if(Nj_souscrit==0){if(Nj_essai!=0) {if(Nj_essai<JOUR_ESSAI) return "ACTIVE"; else return "SUSPENDRE";}}
 else {if(Nj_abon<Nj_souscrit) return "ACTIVE"; else return "SUSPENDRE";}
}

void Serveur::activationDuLogiciel(int periode,QString nom)
{
if(periode!=10){
 m_ui->rapport->append(QString("Demande d'enrégistrement de <strong>"+LOGICIEL.section("_",0,0)+"</strong> pour <strong>%1 ans</strong> par <strong>%2</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(periode).arg(nom));
 m_ui->rapport->ensureCursorVisible();
 textePlus+=QString("Demande d'enrégistrement de <strong>Moutens</strong> pour <strong>%1 ans</strong> par <strong>%2</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(periode).arg(nom);
}
else
{
 m_ui->rapport->append(QString("Demande d'enrégistrement de <strong>"+LOGICIEL.section("_",0,0)+"</strong> pour <strong>1 mois</strong> par <strong>%1</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(nom));
 m_ui->rapport->ensureCursorVisible();
 textePlus+=QString("Demande d'enrégistrement de <strong>Moutens</strong> pour <strong>1 mois</strong> par <strong>%1</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(nom);
}
 if(periode!=10)Njour=periode*365; else Njour=30;
 CLEF_RETOUR="ASK_ACTIVATION";
 envoyerDonnees();
}

QString Serveur::transfertBancaire(QString cb,QString nom)
{
  m_ui->rapport->append(QString("Début du transfert bancaire du compte <strong>%1</strong> de <strong>%2</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(cb).arg(nom));
  m_ui->rapport->ensureCursorVisible();
  textePlus+=QString("Début du transfert bancaire du compte <strong>%1</strong> de <strong>%2</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(cb).arg(nom);

  return "OK";
}

void Serveur::conserverParametresUtilisateurs()
{

 QFile file("ACTVIX/"+LOGICIEL+"/COMPS/"+uneSession.nom+"@"+uneSession.passe+"/"+uneSession.Id+".vix"); QTextStream flux(&file); 

 file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
 if(file.isOpen())
 {
    flux<< QString("%1").arg(Njour)<<endl;
    flux<< QDate::currentDate().toString()<<endl;
    flux<<QString("%1").arg(1);
   file.close();

   construireVue();
  }
}

void Serveur::activation(QString actif)
{
 if(actif=="OK")
    {
     m_ui->rapport->append("Transfert confirmé ! Activation en cours...    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
     textePlus+="Transfert confirmé ! Activation en cours...    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
     texteActivation="ACTIVER";
     conserverParametresUtilisateurs();
     CLEF_RETOUR="ACTIVATION";
     envoyerDonnees();
    }
 else
    {
     m_ui->rapport->append("Echec de transfert !    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
     m_ui->rapport->ensureCursorVisible();
     textePlus+="Echec de transfert !    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
     texteActivation=""; CLEF_RETOUR="ECHEC_ACTIVATION";
    }
}

void Serveur::deconnexionClient()
{
     // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    if(socket==socketAdmin){ADMIN_CONNECTED=false;}
    clients.removeOne(socket); socket->disconnectFromHost(); socket->deleteLater();
    nbreConnectes->setText(QString("Nombre de connectés: %1").arg(clients.size()));

}

void Serveur::enregistrerRapport(QString dt)
{
 m_ui->rapport->append("**************************Rapport Enrégistré !*******************************");
 textePlus+="**************************Rapport Enrégistré !*******************************";
 QDir dir;
 if(!dir.exists("ACTVIX")) dir.mkdir("ACTVIX");
 if(!dir.exists("ACTVIX/RAPPORT")) dir.mkdir("ACTVIX/RAPPORT");
 QFile file("ACTVIX/RAPPORT/"+dt+".vix"); QTextStream flux(&file);
 file.open(QIODevice::WriteOnly |QIODevice::Append| QIODevice::Text);
 flux<<m_ui->rapport->toHtml();
 file.close();
}

void Serveur::on_rap_clicked()
{
  QDate d;
  enregistrerRapport(d.currentDate().toString());
  m_ui->rapport->clear();
}

void Serveur::on_connect_clicked()
{if(ETAT=="STOP"){ouvertureServeur(); ETAT="PLAY";}}

void Serveur::actualiserAcces()
{
    int NBRE_ACTUAL=0;
  QFile fil("ACTVIX/"+LOGICIEL+"/COMPS/"+uneSession.nom+"@"+uneSession.passe+"/coordonnées.vix"); QStringList ligne;QStringList ligne2;
  if(fil.exists())
  {QStringList liste;
   fil.open(QIODevice::ReadOnly);
   if(fil.isOpen()) while(!fil.atEnd()) liste<<fil.readLine(); fil.close();
 if(4<=liste.size())
 {
 if(liste[3].section("\r\n",0,0)==codeActualiser)
  {
   QString nomDoc="ESSAI GRATUIT"; int Nj_essai=0, Nj_abon=0; int tentative;
  QFile file("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()+"/"+uneSession.Id+".vix");
  if(file.exists())
  {file.open(QIODevice::ReadOnly);
   while(!file.atEnd())ligne<<file.readLine(100); file.close();
   if(2<=ligne.size()){QDate d=QDate::fromString(ligne[0].section("\r\n",0,0));
                         Nj_essai=d.daysTo(QDate::currentDate()); tentative=ligne[1].section("\r\n",0,0).toInt();}

    NBRE_ACTUAL=3;
  }

  QFile f("ACTVIX/"+LOGICIEL+"/COMPS/"+uneSession.nom+"@"+uneSession.passe+"/"+uneSession.Id+".vix");  int Nj_souscrit=0;
  QDate d;
 if(f.exists())
 {f.open(QIODevice::ReadOnly);
  while(!f.atEnd()) ligne2<<f.readLine(100); f.close();
  if(3<=ligne2.size())
  {d=QDate::fromString(ligne2[1].section("\r\n",0,0)); Nj_souscrit=ligne2[0].section("\r\n",0,0).toInt();tentative=ligne2[2].section("\r\n",0,0).toInt();}
  Nj_abon= d.daysTo(QDate::currentDate());
  //////////DEterminer le nombre d'actualisation autorisée/////////////////

  if(Nj_souscrit<=365) NBRE_ACTUAL=3;
  if(Nj_souscrit<=730 && 365<Nj_souscrit) NBRE_ACTUAL=6;
  if(Nj_souscrit<=1095 && 730<Nj_souscrit) NBRE_ACTUAL=10;
 }
if(tentative<NBRE_ACTUAL)
 {
  if(Nj_souscrit==0)
    {if(Nj_essai!=0)
     {if(Nj_essai<JOUR_ESSAI)
       {jourRestant=JOUR_ESSAI-Nj_essai;
        QTextStream flux(&file);
        file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
        flux<<ligne[0]; flux<<QString("%1").arg(tentative+1);
        file.close();
         m_ui->rapport->append(QString("Actualisation d'accès de <strong>%1 jours</strong> obténue par <strong>%2      "+QDateTime::currentDateTime().toString()+"</strong>").arg(jourRestant).arg(uneSession.nom));
         m_ui->rapport->ensureCursorVisible();
         textePlus+=QString("Actualisation d'accès de <strong>%1 jours</strong> obténue par <strong>%2      "+QDateTime::currentDateTime().toString()+"</strong>").arg(jourRestant).arg(uneSession.nom);
         CLEF_RETOUR="ACTUALISER_ACCES";
         envoyerDonnees();
       }
     }}
   else
     {if(Nj_abon<Nj_souscrit)
       {jourRestant=Nj_souscrit-Nj_abon; CLEF_RETOUR="ACTUALISER_ACCES";
        QTextStream flux(&f);
        f.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
        flux<<ligne2[0]; flux<<ligne2[1]; flux<<QString("%1").arg(tentative+1);
        f.close();
        m_ui->rapport->append(QString("Actualisation d'accès de <strong>%1 jours</strong> obténue par <strong>%2      "+QDateTime::currentDateTime().toString()+"</strong>").arg(jourRestant).arg(uneSession.nom));
        m_ui->rapport->ensureCursorVisible();
        textePlus+=QString("Actualisation d'accès de <strong>%1 jours</strong> obténue par <strong>%2      "+QDateTime::currentDateTime().toString()+"</strong>").arg(jourRestant).arg(uneSession.nom);
        envoyerDonnees();
       }
      }
     }
   }
 else ///ON ACTIVE LA SECTION AVEC UNE CLEE RECU PAR MAIL
 {
  QStringList lite; QString nomDoc="CleE_kYEs", text;
  QDir dir("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex());
  liste=dir.entryList(QDir::Files);
  for(int i=0;i<liste.size();i++)
      if(codeActualiser==liste[i].section(".",0,0))
      {QFile fich("ACTVIX/"+LOGICIEL+"/"+QCryptographicHash::hash(nomDoc.toUtf8(), QCryptographicHash::Sha1).toHex()+"/"+liste[i]);
        fich.open(QIODevice::ReadOnly);
        if(fich.isOpen()){text=fich.readLine(); Njour=text.section("\r\n",0,0).toInt(); fich.close(); fich.remove();}
        activation("OK");
      }
 }
  }
 }
}

void Serveur::creationDeCompte(Session session)
{
 QDir dossier; codeG="";
  if(!dossier.exists("ACTVIX/"+LOGICIEL+"/COMPS")) dossier.mkdir("ACTVIX/"+LOGICIEL+"/COMPS");
  if(!dossier.exists("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe))
  {
   QDir dir("ACTVIX/"+LOGICIEL+"/COMPS");
   QStringList listeCompte=dir.entryList(QDir::Dirs); listeCompte.removeAt(0); listeCompte.removeAt(0);
   for(int i=0;i<listeCompte.size();i++)
       if(listeCompte[i].section("@",0,0).toUpper()==session.nom.toUpper())
       {textePlus+="Ce nom est déjà occupé";
       m_ui->rapport->append("Ce nom est déjà occupé"); m_ui->rapport->ensureCursorVisible();return;}

   dossier.mkdir("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe);
   QFile file("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe+"/coordonnées.vix"); QTextStream flux(&file);
    file.open(QIODevice::WriteOnly |QIODevice::Truncate| QIODevice::Text);
    flux<< session.nom<<endl;
    flux<< session.passe<<endl;
    flux<< session.number<<endl;
    QTime temps; QDate d;
    QString code=QString("%1").arg(temps.currentTime().second())+QString("%1").arg(temps.currentTime().hour())+QString("%1").arg(temps.currentTime().minute())
         +QString("%1").arg(d.currentDate().day()+d.currentDate().month()+temps.currentTime().second());

    m_ui->rapport->append("Demande d'inscription de la part de <strong>" + uneSession.nom + " </strong>");
    textePlus+="Demande d'inscription de la part de <strong>" + uneSession.nom + " </strong>";
    m_ui->rapport->append("Code généré: <strong>"+code+"</strong>    <strong>");
    textePlus+="Code généré: <strong>"+code+"</strong>    <strong>";
    m_ui->rapport->ensureCursorVisible();

     QByteArray ba=QCryptographicHash::hash(code.toUtf8(), QCryptographicHash::Sha1);
    flux<<ba.toHex();
 file.close();
 CLEF_RETOUR="CODE_SECRET";
 codeG=code;
 envoyerDonnees();
  }
 else
  {CODE="OUI";
   m_ui->rapport->append("Création du compte <strong>" + uneSession.nom + "</strong> intérumpue:<strong> Echec de création !\n</strong><em>compte déjà occupé</em>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
   m_ui->rapport->ensureCursorVisible();
   textePlus+="Création du compte <strong>" + uneSession.nom + "</strong> intérumpue:<strong> Echec de création !\n</strong><em>compte déjà occupé</em>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
   CLEF_RETOUR="INSCRIRE";
   }

 construireVue();
}

void Serveur::confirmerCompte(Session session, QString code)
{
 QFile file("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe+"/coordonnées.vix"); QDir dir;
 file.open(QIODevice::ReadOnly);
 if(file.isOpen()){
 QStringList liste; QString cod;
 while(!file.atEnd()) liste<< file.readLine(100);
 file.close();
 if(4<=liste.size())
 {cod=liste[3];
  if(liste[3].section("\r\n",0,0)==code)
  {CLEF_RETOUR="COMPTE_CREE"; envoyerDonnees();
   m_ui->rapport->append("Création du compte <strong>" + uneSession.nom + "</strong> terminée:<strong> compte créé !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
   m_ui->rapport->ensureCursorVisible();
   textePlus+="Création du compte <strong>" + uneSession.nom + "</strong> terminée:<strong> compte créé !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
  }
  else
  {CLEF_RETOUR="COMPTE_NON_CREE"; CODE="NON";
   file.remove("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe+"/coordonnées.vix");
   dir.rmdir("ACTVIX/"+LOGICIEL+"/COMPS/"+session.nom+"@"+session.passe);
   m_ui->rapport->append("Création du compte <strong>" + uneSession.nom + "</strong> intérumpue:<strong> Echec de création !\n</strong><em>Code non correct !</em>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
   m_ui->rapport->ensureCursorVisible();
   textePlus+="Création du compte <strong>" + uneSession.nom + "</strong> intérumpue:<strong> Echec de création !\n</strong><em>Code non correct !</em>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
   envoyerDonnees();
  }
 }

 m_ui->rapport->append("Code fourni: <strong>"+code+"</strong> Contre "+cod+"    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
 m_ui->rapport->ensureCursorVisible();
 textePlus+="Code fourni: <strong>"+code+"</strong> Contre "+cod+"    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
 }
}

void Serveur::ouvertureSession(QString nom, QString passe)
{
 QDir dir;
 m_ui->rapport->append("Demande d'ouverture de session <strong>" + nom + " </strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
 textePlus+="Demande d'ouverture de session <strong>" + nom + " </strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
 if(dir.exists("ACTVIX/"+LOGICIEL+"/COMPS/"+nom+"@"+passe))
 {
  m_ui->rapport->append("Ouverture de session <strong>" + nom + "</strong> terminée:<strong> Session ouverte !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
  textePlus+="Ouverture de session <strong>" + nom + "</strong> terminée:<strong> Session ouverte !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
  CLEF_RETOUR="CONNECTER"; envoyerDonnees();

  AUTORISATION=suspensionDeLicence(uneSession.Id);
  if(AUTORISATION=="SUSPENDRE"){
  m_ui->rapport->append(QString("Autorisation illégale de <strong>%1</strong> suspendue !!!    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(nom));
  textePlus+=QString("Autorisation illégale de <strong>%1</strong> suspendue !!!    <strong>"+QDateTime::currentDateTime().toString()+"</strong>").arg(nom);
  m_ui->rapport->ensureCursorVisible();}
 }
 else
 {CLEF_RETOUR="NON_CONNECTER";
  m_ui->rapport->append("Ouverture de session <strong>" + nom + "</strong> intérumpue:<strong> Echec d'ouverture !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>");
 textePlus+="Ouverture de session <strong>" + nom + "</strong> intérumpue:<strong> Echec d'ouverture !</strong>    <strong>"+QDateTime::currentDateTime().toString()+"</strong>";
 }
}

void Serveur::construireVue()
{
 QStringList listeDoc, listeFich; QDir doc, docFich;
  QList<QStandardItem *> listeItem, listeI, listeSItem; modele->clear(); QStringList liste;

 ///////////POUR AMK////////////////////
 doc=QDir("ACTVIX/AMK_SERVER"); listeItem.clear(); listeSItem.clear();
 listeDoc=doc.entryList(QDir::Dirs); listeDoc.removeAt(0); listeDoc.removeAt(0);
 listeI<< new QStandardItem("AMK_SERVER");
for(int i=0;i<listeDoc.size();i++)
{
 listeItem<< new QStandardItem(listeDoc[i]);
 docFich=QDir("ACTVIX/AMK_SERVER/"+listeDoc[i]);
if(listeDoc[i]!="COMPS")
 {
  listeFich= docFich.entryList(QDir::Files);
  for(int j=0;j<listeFich.size();j++)
  {listeItem[i]->appendRow(new QStandardItem(listeFich[j]));}

  listeI[0]->appendRow(listeItem[i]);
 }
else
 {
  liste= docFich.entryList(QDir::Dirs); liste.removeAt(0); liste.removeAt(0); listeSItem.clear();
  for(int j=0;j<liste.size();j++)
  {
   listeSItem<< new QStandardItem(liste[j]);
   docFich=QDir("ACTVIX/AMK_SERVER/"+listeDoc[i]+"/"+liste[j]);
   listeFich= docFich.entryList(QDir::Files);
   for(int k=0;k<listeFich.size();k++)
    {listeSItem[j]->appendRow(new QStandardItem(listeFich[k]));}
  }
  for(int j=0;j<listeSItem.size();j++) {listeItem[i]->appendRow(listeSItem[j]);}
  listeI[0]->appendRow(listeItem[i]);
 }
}
 ///////////POUR MOUTENS////////////////////
 doc=QDir("ACTVIX/MOUTENS_SERVER"); listeItem.clear(); listeSItem.clear();
 listeDoc=doc.entryList(QDir::Dirs); listeDoc.removeAt(0); listeDoc.removeAt(0);
 listeI<< new QStandardItem("MOUTENS_SERVER");
for(int i=0;i<listeDoc.size();i++)
{
 listeItem<< new QStandardItem(listeDoc[i]);
 docFich=QDir("ACTVIX/MOUTENS_SERVER/"+listeDoc[i]);
if(listeDoc[i]!="COMPS")
 {
  listeFich= docFich.entryList(QDir::Files);
  for(int j=0;j<listeFich.size();j++)
  {listeItem[i]->appendRow(new QStandardItem(listeFich[j]));}

  listeI[1]->appendRow(listeItem[i]);
 }
else
 {
  liste= docFich.entryList(QDir::Dirs); liste.removeAt(0); liste.removeAt(0); listeSItem.clear();
  for(int j=0;j<liste.size();j++)
  {
   listeSItem<< new QStandardItem(liste[j]);
   docFich=QDir("ACTVIX/MOUTENS_SERVER/"+listeDoc[i]+"/"+liste[j]);
   listeFich= docFich.entryList(QDir::Files);
   for(int k=0;k<listeFich.size();k++)
    {listeSItem[j]->appendRow(new QStandardItem(listeFich[k]));}
  }
  for(int j=0;j<listeSItem.size();j++) {listeItem[i]->appendRow(listeSItem[j]);}
  listeI[1]->appendRow(listeItem[i]);
 }
}
 ///////////POUR OPTIMUS////////////////////
 doc=QDir("ACTVIX/OPTIMUS_SERVER"); listeItem.clear(); listeSItem.clear();
 listeDoc=doc.entryList(QDir::Dirs); listeDoc.removeAt(0); listeDoc.removeAt(0);
 listeI<< new QStandardItem("OPTIMUS_SERVER");
for(int i=0;i<listeDoc.size();i++)
{
 listeItem<< new QStandardItem(listeDoc[i]);
 docFich=QDir("ACTVIX/OPTIMUS_SERVER/"+listeDoc[i]);
if(listeDoc[i]!="COMPS")
 {
  listeFich= docFich.entryList(QDir::Files);
  for(int j=0;j<listeFich.size();j++)
  {listeItem[i]->appendRow(new QStandardItem(listeFich[j]));}

  listeI[2]->appendRow(listeItem[i]);
 }
else
 {
  liste= docFich.entryList(QDir::Dirs); liste.removeAt(0); liste.removeAt(0); listeSItem.clear();
  for(int j=0;j<liste.size();j++)
  {
   listeSItem<< new QStandardItem(liste[j]);
   docFich=QDir("ACTVIX/OPTIMUS_SERVER/"+listeDoc[i]+"/"+liste[j]);
   listeFich= docFich.entryList(QDir::Files);
   for(int k=0;k<listeFich.size();k++)
    {listeSItem[j]->appendRow(new QStandardItem(listeFich[k]));}
  }
  for(int j=0;j<listeSItem.size();j++) {listeItem[i]->appendRow(listeSItem[j]);}
  listeI[2]->appendRow(listeItem[i]);
 }
}

 ///////////POUR ACTIVIX TEST (ALL)////////////////////
 doc=QDir("ACTVIX/ALL"); listeItem.clear(); listeSItem.clear();
 listeDoc=doc.entryList(QDir::Dirs); listeDoc.removeAt(0); listeDoc.removeAt(0);
 listeI<< new QStandardItem("ALL");
for(int i=0;i<listeDoc.size();i++)
{
 listeItem<< new QStandardItem(listeDoc[i]);
 docFich=QDir("ACTVIX/ALL/"+listeDoc[i]);
if(listeDoc[i]!="COMPS")
 {
  listeFich= docFich.entryList(QDir::Files);
  for(int j=0;j<listeFich.size();j++)
  {listeItem[i]->appendRow(new QStandardItem(listeFich[j]));}

  listeI[3]->appendRow(listeItem[i]);
 }
else
 {
  liste= docFich.entryList(QDir::Dirs); liste.removeAt(0); liste.removeAt(0); listeSItem.clear();
  for(int j=0;j<liste.size();j++)
  {
   listeSItem<< new QStandardItem(liste[j]);
   docFich=QDir("ACTVIX/ALL/"+listeDoc[i]+"/"+liste[j]);
   listeFich= docFich.entryList(QDir::Files);
   for(int k=0;k<listeFich.size();k++)
    {listeSItem[j]->appendRow(new QStandardItem(listeFich[k]));}
  }
  for(int j=0;j<listeSItem.size();j++) {listeItem[i]->appendRow(listeSItem[j]);}
  listeI[3]->appendRow(listeItem[i]);
 }
}

///////////POUR RAPPORT////////////////////
 doc=QDir("ACTVIX/RAPPORT"); listeItem.clear(); listeSItem.clear();
 listeI<< new QStandardItem("RAPPORT");
  listeFich= doc.entryList(QDir::Files);
  for(int j=0;j<listeFich.size();j++)
  {listeI[4]->appendRow(new QStandardItem(listeFich[j]));}

for(int i=0;i<listeI.size();i++)modele->appendRow(listeI[i]);

QStringList labels; labels<<"Dossiers Créés dans Activix";
modele->setHorizontalHeaderLabels(labels);
 m_ui->dossier->setModel(modele);
}

void Serveur::selection()
{
  QItemSelectionModel *selection = m_ui->dossier->selectionModel();
  QModelIndex indexElementSelectionne = selection->currentIndex(); // Reccuperation de l'index du choix
  QStandardItem *choix= modele->itemFromIndex(indexElementSelectionne); // recuperation du item cacher sous cet index

  QString fichier=choix->text();

if(choix->text().contains(".", Qt::CaseInsensitive))
{
 if(choix->parent()->text()=="RAPPORT")
     {
      QFile f("ACTVIX/"+(choix->parent())->text()+"/"+fichier);
      f.open(QIODevice::ReadOnly);m_ui->visual->setHtml(f.readAll()); f.close();  m_ui->boiteRapport->show();
     }
  if((choix->parent())->text()=="49da57170c57ead29d1982dbb25dced1965f9411")
   {
      QFile f("ACTVIX/"+(choix->parent())->parent()->text()+"/"+(choix->parent())->text()+"/"+fichier);
      f.open(QIODevice::ReadOnly);m_ui->visual->setHtml(f.readAll()); f.close();  m_ui->boiteRapport->show();
    }
  if((choix->parent())->text()!="49da57170c57ead29d1982dbb25dced1965f9411" && (choix->parent())->text()!="RAPPORT")
  {
   QFile f("ACTVIX/"+((choix->parent())->parent())->parent()->text()+"/"+(choix->parent())->parent()->text()+"/"+(choix->parent())->text()+"/"+fichier);
      f.open(QIODevice::ReadOnly);m_ui->visual->setHtml(f.readAll()); f.close();  m_ui->boiteRapport->show();
  }
 }
}

void Serveur::lireCryptage(QString texte)
{
 int deplacementLettre=5, substitutionLettre=10;
 QTime tim,T; tim=T.currentTime(); deplacementLettre=int((deplacementLettre+tim.minute())/4) + 3;
 substitutionLettre+=tim.second();
 codeCrypt="";
 cryptageLettre(texte, deplacementLettre, substitutionLettre);
}

void Serveur::cryptageLettre(QString texte, int decallage, int nombreLettre)
{
     int temoin =0,decNombre; QChar espace(' '); decNombre=decallage; texteCrypter="";
     QString elt; QList<QChar>  texteACrypter;
     for(int i=0; i<texte.size(); i++){elt= texte.mid(i,1); texteACrypter<<*elt.data();}

     for(int i=0; i<texteACrypter.size(); i++) // ...........On parcours toute la chaine du texte lu
     { temoin =0;
       if(texteACrypter[i] == espace)
         {for(int k=0; k< nombreLettre; k++)
           { texteCrypter +=":"; temoin=1;}} // Si le caractère est un vide
       if(texteACrypter[i].isLetter() && texteACrypter[i].isUpper())// Si le carractère est une lettre majuscule
        {
            for(int j=0; j<26; j++)
             { if(crypt[j]== texteACrypter[i] && j+decallage<26)
                for(int k=0; k< nombreLettre; k++) texteCrypter += crypt[j+decallage];

               if(crypt[j]== texteACrypter[i] && j+decallage>=26)
                  for(int k=0; k< nombreLettre; k++) texteCrypter += crypt[decallage-26+j];
             }
             temoin = 1;
        }
        if(texteACrypter[i].isLetter() && texteACrypter[i].isLower())// si le carractère est une lettre minuscule
        {
             for(int j=0; j<26; j++)
             { if(cryptLower[j]== texteACrypter[i] && j+decallage<26)
                for(int k=0; k< nombreLettre; k++) texteCrypter += cryptLower[j+decallage];

               if(cryptLower[j]== texteACrypter[i] && j+decallage>=26)
                  for(int k=0; k< nombreLettre; k++) texteCrypter += cryptLower[decallage-26+j];
             }
              temoin = 1;
        }
        if(texteACrypter[i].isNumber())// Si le carractère est un nombre
        { if(decNombre>10) decNombre -=10; if(decNombre>10) decNombre -=10;
             for(int j=0; j<10; j++)
             { if(cryptNombre[j]== texteACrypter[i] && j+decNombre<10)
                for(int k=0; k< nombreLettre; k++) texteCrypter += cryptNombre[decNombre];

               if(cryptNombre[j]== texteACrypter[i] && j+decNombre>=10)
                  for(int k=0; k< nombreLettre; k++) texteCrypter += cryptNombre[decNombre-10+j];
             }
              temoin = 1;
         }

       if(temoin == 0){for(int k=0; k< nombreLettre; k++) texteCrypter += texteACrypter[i]; }// Si le caractère est autre qu'un nbre ou une lettre
     }
}

Serveur::~Serveur()
{
    delete m_ui;delete serveur; delete modele;
    delete etatServeur; delete nbreConnectes;
    for(int i=0;i<clients.size(); i++) delete clients[i];
}

void Serveur::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
