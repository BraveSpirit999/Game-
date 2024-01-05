#ifndef STYLEHELPER_H
#define STYLEHELPER_H
#include<QString>
class StyleHelper
{
public:
    static QString getDarkMainWidgetStyle();
    static QString getLightMainWidgetStyle();
    static QString getDarkNameGameStyle();
    static QString getLightNameGameStyle();
    static QString getLightStartButtonStyle();
    static QString getDarkStartButtonStyle();
    static QString getStartButtonGameStyle();
    static QString getDarkLeftButtonStyle();
    static QString getDarkRightButtonStyle();
    static QString getLightLeftButtonStyle();
    static QString getLightRightButtonStyle();
    static QString getDarkLeftButtonActiveStyle();
    static QString getDarkRightButtonActiveStyle();
    static QString getLightLeftButtonActiveStyle();
    static QString getLightRightButtonActiveStyle();
    static QString getTabWidgetStyle();
    static QString getGameStyleButton();

    static QString getBlankButtonStyle();
    static QString getCrossNormalStyle();
    static QString getCrossVictoryStyle();
    static QString getCrossLostStyle();
    static QString getZeroNormalStyle();
    static QString getZeroVictoryStyle();
    static QString getZeroLostStyle();

    static QString getNormalMessageStyle();
    static QString getVictoryMessageStyle();
    static QString getLostMessageStyle();
    static QString getAboutTextStyle();
};

#endif // STYLEHELPER_H
