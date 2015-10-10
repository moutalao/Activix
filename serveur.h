#ifndef SERVEUR_H
#define SERVEUR_H
#include <QtGui>
#include <QtNetwork>

namespace Ui {
    class Serveur;
}

struct Session
{
   QString nom;
   QByteArray passe;
   QString number;
   QByteArray Id;
};

class Serveur : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY(Serveur)
public:
    explicit Serveur(QWidget *parent = 0);
    virtual ~Serveur();
    void creationDeCompte(Session session);
    void envoyerDonnees();
    void envoyerDonneesAdmin();
    void ouvertureSession(QString nom, QString passe);
    void confirmerCompte(Session session, QString code);
    void activationDuLogiciel(int p, QString nom);
    void activation(QString actif);
    QString transfertBancaire(QString cb,QString nom);
    void enregistrerRapport(QString dt);
    void dossierDEssaiGratuit(QString Id);
    void conserverParametresUtilisateurs();
    QString suspensionDeLicence(QString Id);
    void construireVue();
    void actualiserAcces();
    void creerRepertoires(QString rep);
    void saveConfig();
    void openConfig();
    void fonctionAdministrateur();
    void initialiserCryptage();
    void cryptageLettre(QString texte, int decallage, int nombreLettre);
    void lireCryptage(QString texte);
    void creerClef(QString cK);
    void supprimerClef(QString ck);

protected:
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

public slots:
            void ouvertureServeur();
            void nouvelleConnexion();
            void donneesRecues();
            void deconnexionClient();
            void on_connect_clicked();
            void on_rap_clicked();
            void selection();

private:
    Ui::Serveur *m_ui;
        QTcpServer *serveur;
        QLabel *etatServeur, *nbreConnectes;
        QList<QTcpSocket *> clients;
        QTcpSocket *socketAdmin;
        quint16 tailleMessage;

        QString ETAT, CLEF, CLEF_RETOUR, CODE, LOGICIEL;
        Session uneSession;
        QString AUTORISATION;
        QString codeG;
        int Njour, jourRestant;
        QByteArray texteActivation, codeActualiser;
        QStandardItemModel *modele;

        bool ADMIN_CONNECTED;
        QString textePlus, CLEF_ADMIN;
        QStringList crypt, cryptLower, cryptNombre;
        QString texteCrypter, codeCrypt;
};

#endif // SERVEUR_H
