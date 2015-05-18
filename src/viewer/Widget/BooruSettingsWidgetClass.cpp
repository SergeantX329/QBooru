#include <iostream>
#include <ctime>

#include "BooruSettingsWidgetClass.h"
#include "booruSettingsTabClass.h"
#include "../classBooruSite.h"
#include "../json/json.h"
#include "widget.h"
#include "fonctions_all.h"

BooruSettings::BooruSettings(BooruSettingsTab *parent)
{
    layoutMain = new QVBoxLayout;
    parentWidget = parent;
    Json::Value root = loadJSONFile(BOORU_LIST);
    QString buffer;
    int i=0;

    groupBoxMainSettings = new QGroupBox("Main Settings", this);
    layoutGroupBoxMainSettings = new QGridLayout;

        checkBoxPreset = new QCheckBox("Presets",this);
        checkBoxPreset->setChecked(false);

        comboBoxPreset = new QComboBox(this);
        comboBoxPreset->setDisabled(true);
        layoutGroupBoxMainSettings->addWidget(checkBoxPreset,0,2);
        layoutGroupBoxMainSettings->addWidget(comboBoxPreset,0,3);

        while(root["boorus"][i].isObject())
        {
            buffer = root["boorus"][i]["name"].asString().c_str();
            buffer[0].toUpper();
            comboBoxPreset->addItem(buffer);
            i++;
        }

        labelBooruName = new QLabel("Booru Name",this);
        lineEditBooruName = new QLineEdit(this);
        layoutGroupBoxMainSettings->addWidget(labelBooruName,0,0);
        layoutGroupBoxMainSettings->addWidget(lineEditBooruName,0,1);

        labelBooruURL = new QLabel("Booru Main URL",this);
        lineEditBooruURL = new QLineEdit(this);
        layoutGroupBoxMainSettings->addWidget(labelBooruURL,1,0);
        layoutGroupBoxMainSettings->addWidget(lineEditBooruURL,1,1);

        labelBooruType = new QLabel("Booru Type",this);
        comboBoxBooruType = new QComboBox(this);
        comboBoxBooruType->addItem("Derpibooru (not recommended, is being recoded)");
        comboBoxBooruType->addItem("Gelbooru");
        comboBoxBooruType->addItem("Moebooru");
        comboBoxBooruType->addItem("Danbooru");
        layoutGroupBoxMainSettings->addWidget(labelBooruType,2,0);
        layoutGroupBoxMainSettings->addWidget(comboBoxBooruType,2,1);
        comboBoxBooruType->setCurrentIndex(GELBOORU_TYPE);

    groupBoxOptionalSettings = new QGroupBox("Optional Settings", this);
    groupBoxOptionalSettings->setCheckable(true);
    groupBoxOptionalSettings->setChecked(false);
    layoutGroupBoxOptionalSettings = new QGridLayout;

        labelBooruDownloadPath = new QLabel("Download Path",this);
        lineEditBooruDownloadPath = new QLineEdit(this);
        layoutGroupBoxOptionalSettings->addWidget(labelBooruDownloadPath,0,0);
        layoutGroupBoxOptionalSettings->addWidget(lineEditBooruDownloadPath,0,1);

    groupBoxAdvancedSettings = new QGroupBox("Optional Settings", this);
    groupBoxAdvancedSettings->setCheckable(true);
    groupBoxAdvancedSettings->setChecked(false);
    layoutGroupBoxAdvancedSettings = new QGridLayout;

        labelBooruSearchUrl = new QLabel("Search URL",this);
        lineEditBooruSearchUrl = new QLineEdit(this);
        layoutGroupBoxAdvancedSettings->addWidget(labelBooruSearchUrl,0,0);
        layoutGroupBoxAdvancedSettings->addWidget(lineEditBooruSearchUrl,0,1);

        labelBooruShowIndexUrl = new QLabel("Posts' base URL",this);
        lineEditBooruShowIndexUrl = new QLineEdit(this);
        layoutGroupBoxAdvancedSettings->addWidget(labelBooruShowIndexUrl,1,0);
        layoutGroupBoxAdvancedSettings->addWidget(lineEditBooruShowIndexUrl,1,1);

        labelBooruTagsUrl = new QLabel("Taglist URL",this);
        lineEditBooruTagsUrl = new QLineEdit(this);
        layoutGroupBoxAdvancedSettings->addWidget(labelBooruTagsUrl,2,0);
        layoutGroupBoxAdvancedSettings->addWidget(lineEditBooruTagsUrl,2,1);

     groupBoxMainSettings->setLayout(layoutGroupBoxMainSettings);
     groupBoxOptionalSettings->setLayout(layoutGroupBoxOptionalSettings);
     groupBoxAdvancedSettings->setLayout(layoutGroupBoxAdvancedSettings);

     layoutMain->addWidget(groupBoxMainSettings);
     layoutMain->addWidget(groupBoxOptionalSettings);
     layoutMain->addWidget(groupBoxAdvancedSettings);

     saveButton = new QPushButton("Save");

     connect(comboBoxPreset,SIGNAL(currentIndexChanged(int)),this,SLOT(loadPreset(int)));
     connect(checkBoxPreset,SIGNAL(clicked(bool)),comboBoxPreset,SLOT(setEnabled(bool)));

     layoutMain->addWidget(saveButton);

     setLayout(layoutMain);

}

BooruSettings::~BooruSettings()
{
    clearLayout(layoutMain);
}

void BooruSettings::editBooru()
{
    /*!
     Saves an edited booru into your config file, replacing its previous configuration
     */
    BooruSite *booru;
    int index = parentWidget->comboBoxBooru->currentIndex();

    if(!groupBoxAdvancedSettings->isChecked() && !groupBoxOptionalSettings->isChecked())
    {
        booru = new BooruSite(lineEditBooruName->text().toStdString(),
                              lineEditBooruURL->text().toStdString(),
                              comboBoxBooruType->currentIndex(),
                              index);
    }
    else if(!groupBoxAdvancedSettings->isChecked() && groupBoxOptionalSettings->isChecked())
    {
        booru = new BooruSite(lineEditBooruName->text().toStdString(),
                              lineEditBooruURL->text().toStdString(),
                              lineEditBooruDownloadPath->text().toStdString() ,
                              comboBoxBooruType->currentIndex(),
                              index);
    }
    else if(groupBoxAdvancedSettings->isChecked() && !groupBoxOptionalSettings->isChecked())
    {
        booru = new BooruSite(lineEditBooruName->text().toStdString(),
                              lineEditBooruURL->text().toStdString(),
                              lineEditBooruSearchUrl->text().toStdString(),
                              lineEditBooruTagsUrl->text().toStdString(),
                              lineEditBooruShowIndexUrl->text().toStdString(),
                              lineEditBooruDownloadPath->text().toStdString() ,
                              comboBoxBooruType->currentIndex(),
                              index);
    }
    else
    {
        booru = new BooruSite(lineEditBooruName->text().toStdString(),
                              lineEditBooruURL->text().toStdString(),
                              lineEditBooruSearchUrl->text().toStdString(),
                              lineEditBooruTagsUrl->text().toStdString(),
                              lineEditBooruShowIndexUrl->text().toStdString(),
                              std::string(std::string(downloadPath()) + lineEditBooruName->text().toStdString()) ,
                              comboBoxBooruType->currentIndex(),
                              index);
    }


    booru->saveBooruSite();
    delete booru;
}

void BooruSettings::loadBooru(int index)
{
    /*!
     Loads a saved Booru into the Widget, allowing for edits

     \param index of the booru to load
     */
    Json::Value root = loadJSONFile(CONF_FILE);

    if(index < root["settings"]["booru_number"].asInt())
    {
        lineEditBooruName->setText(QString(root["boorus"][index]["name"].asCString()));

        lineEditBooruURL->setText(QString(root["boorus"][index]["base_url"].asCString()));

        comboBoxBooruType->setCurrentIndex(root["boorus"][index]["siteTypeInt"].asInt());

        lineEditBooruDownloadPath->setText(QString(root["boorus"][index]["download_path"].asCString()));

        lineEditBooruSearchUrl->setText(QString(root["boorus"][index]["search_url"].asCString()));

        lineEditBooruShowIndexUrl->setText(QString(root["boorus"][index]["show_index_url"].asCString()));

        lineEditBooruTagsUrl->setText(QString(root["boorus"][index]["tag_url"].asCString()));
    }
}

void BooruSettings::loadPreset(int index)
{
    /*!
     Loads a preset into the Widget, allowing for edits and save

     \param index of the booru to load
     */
    Json::Value root = loadJSONFile(BOORU_LIST);

    lineEditBooruName->setText(QString(root["boorus"][index]["name"].asCString()));

    lineEditBooruURL->setText(QString(root["boorus"][index]["base_url"].asCString()));

    comboBoxBooruType->setCurrentIndex(root["boorus"][index]["siteTypeInt"].asInt());
}
