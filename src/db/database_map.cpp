
#include "database_map.h"
#include <iostream>

DatabaseMap::DatabaseMap(const std::string &sFilepath) {
    TAG = "DatabaseMap";
    m_sFilepath = sFilepath;
    m_pDatabase = nullptr;
}

bool DatabaseMap::connect() {
    int rc = sqlite3_open(m_sFilepath.c_str(), &m_pDatabase);
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(m_pDatabase));
        sqlite3_close(m_pDatabase);
        return false;
    }

    std::string sSql = 
                "CREATE TABLE IF NOT EXISTS map_objects("
                "   id INT, x INT, y INT, weight INT, height INT,"
                "   health INT, layer INT, canbe_destroyed INT,"
                "   image TEXT, current_state TEXT"
                ");"
    ;

    char *err_msg = 0;
    rc = sqlite3_exec(m_pDatabase, sSql.c_str(), 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        sqlite3_close(m_pDatabase);
        return false;
    } 

    return true;
}

std::vector<GameMapObject *> DatabaseMap::getMapObjectsRect(int x, int y, int w, int h) {
    std::vector<GameMapObject *> ret;
    sqlite3_stmt *res;
    char * sql = sqlite3_mprintf(
        "SELECT id,x,y,weight,height,health,layer,canbe_destroyed,image,current_state FROM map_objects WHERE x >= %d AND x <= %d AND y >= %d AND y <= %d;",
        x, x + w,
        y, y + h
    );
    std::cout << "SQL: " << sql << std::endl;
    int rc = sqlite3_prepare_v2(m_pDatabase, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_pDatabase));
        // sqlite3_close(m_pDatabase);
        return ret;
    }
    rc = sqlite3_step(res);
    int ncols = sqlite3_column_count(res);

    while (rc == SQLITE_ROW) {
        int nId = sqlite3_column_int(res, 0);
        int nX = sqlite3_column_int(res, 1);
        int nY = sqlite3_column_int(res, 2);
        int nWidth = sqlite3_column_int(res, 3);
        int nHeight = sqlite3_column_int(res, 4);
        int nHealth = sqlite3_column_int(res, 5);
        int nLayer = sqlite3_column_int(res, 6);
        bool bCanbeDestroyed  = sqlite3_column_int(res, 7);
        std::string sImageTexture = std::string((const char*)sqlite3_column_text(res, 8));
        std::string sCurrentState = std::string((const char*)sqlite3_column_text(res, 9));
        ret.push_back(new GameMapObject(
            nId, nX, nY, nWidth, nHeight,
            nHealth, nLayer,
            bCanbeDestroyed, sImageTexture, sCurrentState
        ));
        rc = sqlite3_step(res);
    }
    sqlite3_finalize(res);

    return ret;
}