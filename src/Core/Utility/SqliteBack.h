// SqliteBack.h
#if !defined(_SQLITEBACK_H)
#define _SQLITEBACK_H

#include "EventManager.h"
#include "SqliteDb.h"

#include <list>
#include <string>

// An EventManager backend that writes data to an sqlite database.  Identically
// named events have their data placed as rows in a single table.
class SqliteBack: public EventBackend {
  public:
    // Creates a new sqlite backend that will write to (or overwrite) a file.
    //
    // @param filename the filepath (including name) to write the sqlite file.
    SqliteBack(std::string filename);
    
    virtual ~SqliteBack();
  
    // Write events immediately to the database as a single transaction.
    //
    // @param events group of events to write to the database together.
    void notify(EventList events);

    /// Returns a unique name for this backend
    std::string name();

    /// Finishes any incomplete tasks and closes the database/file.
    void close();

  private:
    bool tableExists(event_ptr e); 

    std::string valType(boost::any v); 

    std::string valData(boost::any v);

    /// Queue up a table-create command for e.
    void createTable(event_ptr e);

    void writeEvent(event_ptr e);
    
    /// Execute all pending commands
    void flush();

    /// A pointer to the database managed by the SqliteBack class 
    SqliteDb* db_;

    /// Stores the database's name, declared during construction. 
    std::string path_;

    StrList cmds_;

    StrList tbl_names_;
};

#endif
