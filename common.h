#ifndef COMMON_H
#define COMMON_H

//定义天气api的相关数据
#define WEATHER_URL "http://gfeljm.tianqiapi.com/api?unescape=1&version=%1&appid=%2&appsecret=%3"
#define WEATHER_VERSION "v91"
#define WEATHER_APPID "36644469"
#define WEATHER_APPSECRET "bWovZml1"

//定义百度地图的api的相关数据
#define BAIDU_MAP_URL "https://api.map.baidu.com/staticimage/v2?ak=%1&width=450&height=300&scaler=2&center=%2,%3&zoom=%4"
#define BAIDU_MAP_AK "KMUAppZBziR9C8YDEjmWW8M76No2knz0"


#define BAIDU_MAP_IP_URL "https://api.map.baidu.com/location/ip?ip=%1&coor=bd09ll&ak=%2"

// 新增路径规划API
#define BAIDU_ROUTE_URL "https://api.map.baidu.com/directionlite/v1/driving?ak=%1&origin=%2,%3&destination=%4,%5&coord_type=bd09ll&radius=1500"
#endif // COMMON_H

