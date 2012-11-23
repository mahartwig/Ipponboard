#ifndef BASE__MAINWINDOW_H_
#define BASE__MAINWINDOW_H_

#include "mainwindowbase.h"
#ifndef TEAM_VIEW
#include "../core/fighter.h"
#endif

#include <boost/shared_ptr.hpp>

class MainWindow : public MainWindowBase
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

    virtual const char* GetConfigFileName() const override;
	virtual void Init() sealed;

protected:
    //virtual void changeEvent(QEvent* e) override;
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
	void update_info_text_color(const QColor& color, const QColor& bgColor) override;
    void update_text_color_blue(const QColor& color, const QColor& bgColor) override;
    void update_text_color_white(const QColor& color, const QColor& bgColor) override;
    virtual void update_fighter_name_font(const QFont&) override;
    virtual void update_views() override;
#ifdef TEAM_VIEW
    void update_club_views();
    void UpdateFightNumber_();
    void update_score_screen();
    void WriteScoreToHtml_();
    virtual void write_specific_settings(QSettings& settings) sealed;
    virtual void read_specific_settings(QSettings& settings) sealed;
#else
    void update_fighter_name_completer(const QString& weight);
    void update_fighters(const QString& s);
#endif

private slots:
#ifdef TEAM_VIEW
    void on_tableView_tournament_list1_customContextMenuRequested(QPoint const& pos);
    void on_tableView_tournament_list2_customContextMenuRequested(QPoint const& pos);
    void on_actionScore_Control_triggered();
    void on_actionScore_Screen_triggered();
    void on_button_pause_clicked();
    void on_button_current_round_clicked(bool checked);
    void on_actionSet_Round_Time_triggered();
    void on_pushButton_copySwitched_pressed();
    void on_toolButton_weights_pressed();
    void slot_copy_cell_content_list1();
    void slot_copy_cell_content_list2();
    void slot_paste_cell_content_list1();
    void slot_paste_cell_content_list2();
    void slot_clear_cell_content_list1();
    void slot_clear_cell_content_list2();
    void Print(QPrinter* p)
    {
        QTextEdit e(m_htmlScore, this);
        e.document()->print(p);
    }
    void on_tabWidget_currentChanged(int index);
    void on_actionManage_Clubs_triggered();
    void on_actionLoad_Demo_Data_triggered();
    void on_button_prev_clicked();
    void on_button_next_clicked();
    void on_comboBox_mode_currentIndexChanged(const QString& s);
    void on_comboBox_club_host_currentIndexChanged(const QString& s);
    void on_comboBox_club_home_currentIndexChanged(const QString& s);
    void on_comboBox_club_guest_currentIndexChanged(const QString& s);
    void on_actionPrint_triggered();
    void on_actionExport_triggered();
#else
    void on_actionManage_Classes_triggered();
    void on_comboBox_weight_currentIndexChanged(const QString&);
    void on_comboBox_name_white_currentIndexChanged(const QString&);
    void on_comboBox_name_blue_currentIndexChanged(const QString&);
    void on_checkBox_golden_score_clicked(bool checked);
    void on_comboBox_weight_class_currentIndexChanged(const QString&);
    void on_actionImportList_triggered();
    void on_actionExportList_triggered();
#endif
    void on_actionReset_Scores_triggered();
    virtual bool EvaluateSpecificInput(FMlib::Gamepad const* pGamepad) override;

private:
#ifdef TEAM_VIEW
    void update_weights(QString weightString);
    void on_tableView_customContextMenuRequested(QTableView* pTableView,
                                                 QPoint const& pos,
                                                 const char* copySlot,
                                                 const char* pasteSlot,
                                                 const char* clearSlot);
    void copy_cell_content(QTableView* pTableView);
    void paste_cell_content(QTableView* pTableView);
    void clear_cell_content(QTableView* pTableView);
    static QString get_template_file(QString const& mode);
    static QString get_full_mode_title(QString const& mode);

    boost::shared_ptr<Ipponboard::ScoreScreen> m_pScoreScreen;
    boost::shared_ptr<Ipponboard::ClubManager> m_pClubManager;
//    std::vector<QTableWidgetItem> fighters_home;
//    std::vector<QTableWidgetItem> fighters_guest;
    QString m_htmlScore;
    QString m_mode;
    QString m_host;
#else
    boost::shared_ptr<Ipponboard::FightCategoryMgr> m_pCategoryManager;
    QStringList m_CurrentFighterNames;
#endif
};

#endif  // BASE__MAINWINDOW_H_
