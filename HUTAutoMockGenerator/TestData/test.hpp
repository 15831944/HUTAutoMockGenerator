class CRRecordSet : public CObject
{
public:
	RN1UTILS_API CRRecordSet();           // protected constructor used by dynamic creation
	RN1UTILS_API virtual ~CRRecordSet();

public:
	static CString sqlState;
	RN1UTILS_API static BOOL silentMode;	// for Oracle support
	RN1UTILS_API static BOOL afxNotPresent;
	static BOOL authenticationFailure;
	static BOOL RDFformat; //For Unicode support

// Operations
public:
	RN1UTILS_API const CString GetTableName() const { return tableName; }
	RN1UTILS_API const CString GetVisibleTableName() const;
	RN1UTILS_API const BOOL IsTempTable() const { return (tableName.Find(_T("#")) >= 0); }
	RN1UTILS_API BOOL SetTableName(const CString& tableName);
	RN1UTILS_API ID GetNewId();  // returns the new ID
	RN1UTILS_API ID GetNewIdBlock(int count); // returns the starting ID of the block

	RN1UTILS_API const CString GetWhereClause() const { return whereClause; }
	RN1UTILS_API BOOL SetWhereClause(const CString& whereClause);

	RN1UTILS_API BOOL ResetParameters();
	RN1UTILS_API BOOL AddParameter(RFIELD rType, CString rgbValue);
	RN1UTILS_API BOOL StartBinding(HSTMT& hstmt);

	RN1UTILS_API const CString GetOrderBy() const { return orderBy; }
	RN1UTILS_API BOOL SetOrderBy(const CString& orderBy);

	RN1UTILS_API const CString GetGroupBy() const { return groupBy; }
	RN1UTILS_API BOOL SetGroupBy(const CString& groupBy);

	RN1UTILS_API CRColumnArray& GetColumns() { return cols; }

	RN1UTILS_API CString GetSQLSelectString(BOOL notDistinct, BOOL fastFirstRow = FALSE) const;
	RN1UTILS_API CString GetSQLCountString(BOOL notDistinct) const;
	RN1UTILS_API CString GetSQLAddString() const;
	RN1UTILS_API CString GetSQLDeleteString(const CString& deleteWhereClause) const;
	RN1UTILS_API CString GetSQLUpdateString(const CString& updateWhereClause) const;
	RN1UTILS_API CString GetColumnList(BOOL values = FALSE) const;
	RN1UTILS_API CString GetValueList() const;

	RN1UTILS_API BOOL IsJoin() const { return join; }

	RN1UTILS_API BOOL BuildLikePattern(CString& csPattern, BOOL fStartsWith, BOOL fNumeric);
	RN1UTILS_API BOOL BuildLikePatternValue(CString& csPattern, BOOL fStartsWith, BOOL fNumeric);

	RN1UTILS_API BOOL AddColumn(const CString& newColumn, RFIELD rtype);
	RN1UTILS_API BOOL AddColumnForText(const CString& newColumn, RFIELD rtype, const CString& text);
	RN1UTILS_API BOOL AddTimestampColumn(const CString& newColumn);
	RN1UTILS_API BOOL RemoveAllColumns();
	RN1UTILS_API BOOL IsColumn(const CString& column) const;
	RN1UTILS_API int ColumnPos(const CString& column) const;
	RN1UTILS_API int GetNumColumns() const;
	RN1UTILS_API CRColumn* GetColumn(const CString& column) const;
	RN1UTILS_API CRColumn* GetColumn(int column) const;

	RN1UTILS_API BOOL IsNull(const CString& column) const;
	RN1UTILS_API BOOL IsNull(int column) const;

	RN1UTILS_API CString GetText(const CString& column) const;
	RN1UTILS_API CString GetText(int column) const;
	RN1UTILS_API std::wstring GetWideText(int column) const;
	RN1UTILS_API CString GetCombo(const CString& column) const { return GetText(column); }
	RN1UTILS_API CString GetCombo(int column) const { return GetText(column); }
	RN1UTILS_API CString GetMemo(const CString& column) const { return GetText(column); }
	RN1UTILS_API CString GetMemo(int column) const { return GetText(column); }
	RN1UTILS_API U8 GetChoice(const CString& column) const;
	RN1UTILS_API U8 GetChoice(int column) const;
	RN1UTILS_API BOOL GetBoolean(const CString& column) const;
	RN1UTILS_API BOOL GetBoolean(int column) const;
	RN1UTILS_API S32 GetInteger(const CString& column) const;
	RN1UTILS_API S32 GetInteger(int column) const;
	RN1UTILS_API ID GetId(const CString& column) const;
	RN1UTILS_API ID GetId(int column) const;
	RN1UTILS_API OLDID GetOldId(const CString& column) const;
	RN1UTILS_API OLDID GetOldId(int column) const;
	RN1UTILS_API double GetFloat(const CString& column) const;
	RN1UTILS_API double GetFloat(int column) const;
	RN1UTILS_API double GetCurrency(const CString& column) const { return GetFloat(column); }
	RN1UTILS_API double GetCurrency(int column) const { return GetFloat(column); }
	RN1UTILS_API DATE_STRUCT GetDate(const CString& column) const;
	RN1UTILS_API DATE_STRUCT GetDate(int column) const;
	RN1UTILS_API TIME_STRUCT GetTime(const CString& column) const;
	RN1UTILS_API TIME_STRUCT GetTime(int column) const;
	RN1UTILS_API TIMESTAMP_STRUCT GetTimestamp(const CString& column) const;
	RN1UTILS_API TIMESTAMP_STRUCT GetTimestamp(int column) const;
	RN1UTILS_API void* GetBinary(const CString& column, int& length) const;
	RN1UTILS_API void* GetBinary(int column, int& length) const;
	RN1UTILS_API void* GetBinaryAndRelease(int column, int& length) const; // caller is responsible for freeing buffer using delete[]
	RN1UTILS_API void* GetGraphic(const CString& column, int& length) const { return GetBinary(column, length); }
	RN1UTILS_API void* GetGraphic(int column, int& length) const { return GetBinary(column, length); }
	RN1UTILS_API CObject* GetObject(const CString& column, CObject* pObject = NULL, bool close = false);
	RN1UTILS_API CObject* GetObject(int column, CObject* pObject = NULL, bool close = false);
	RN1UTILS_API CString GetValueText(const CString& column) const;
	RN1UTILS_API CString GetValueText(int column, int decimalPlaces = -1) const;
	RN1UTILS_API void GetVariant(const CString& column, VARIANT* var) const;
	RN1UTILS_API void GetVariant(int column, VARIANT* var) const;

	RN1UTILS_API BOOL SetNull(const CString& column) const;
	RN1UTILS_API BOOL SetNull(int column) const;
	RN1UTILS_API BOOL SetText(const CString& column, const CString& value) const;
	RN1UTILS_API BOOL SetText(int column, const CString& value) const;
	RN1UTILS_API BOOL SetCombo(const CString& column, const CString& value) const { return SetText(column, value); }
	RN1UTILS_API BOOL SetCombo(int column, const CString& value) const { return SetText(column, value); }
	RN1UTILS_API BOOL SetMemo(const CString& column, const CString& value) const { return SetText(column, value); }
	RN1UTILS_API BOOL SetMemo(int column, const CString& value) const { return SetText(column, value); }
	RN1UTILS_API BOOL SetChoice(const CString& column, U8 value) const;
	RN1UTILS_API BOOL SetChoice(int column, U8 value) const;
	RN1UTILS_API BOOL SetBoolean(const CString& column, BOOL value) const;
	RN1UTILS_API BOOL SetBoolean(int column, BOOL value) const;
	RN1UTILS_API BOOL SetInteger(const CString& column, S32 value) const;
	RN1UTILS_API BOOL SetInteger(int column, S32 value) const;
	RN1UTILS_API BOOL SetId(const CString& column, const ID& value) const;
	RN1UTILS_API BOOL SetId(int column, const ID& value) const;
	RN1UTILS_API BOOL SetFloat(const CString& column, double value) const;
	RN1UTILS_API BOOL SetFloat(int column, double value) const;
	RN1UTILS_API BOOL SetCurrency(const CString& column, double value) const { return SetFloat(column, value); }
	RN1UTILS_API BOOL SetCurrency(int column, double value) const { return SetFloat(column, value); }
	RN1UTILS_API BOOL SetDate(const CString& column, const DATE_STRUCT& value) const;
	RN1UTILS_API BOOL SetDate(int column, const DATE_STRUCT& value) const;
	RN1UTILS_API BOOL SetTime(const CString& column, const TIME_STRUCT& value) const;
	RN1UTILS_API BOOL SetTime(int column, const TIME_STRUCT& value) const;
	RN1UTILS_API BOOL SetTimestamp(const CString& column, const TIMESTAMP_STRUCT& value) const;
	RN1UTILS_API BOOL SetTimestamp(int column, const TIMESTAMP_STRUCT& value) const;
	RN1UTILS_API BOOL SetBinary(const CString& column, void* pValue, int length) const;
	RN1UTILS_API BOOL SetBinary(int column, void* pValue, int length) const;
	RN1UTILS_API BOOL SetGraphic(const CString& column, void* pValue, int length) const { return SetBinary(column, pValue, length); }
	RN1UTILS_API BOOL SetGraphic(int column, void* pValue, int length) const { return SetBinary(column, pValue, length); }
	RN1UTILS_API BOOL SetObject(const CString& column, CObject* pObject, BOOL serializeOnly = FALSE) const;
	RN1UTILS_API BOOL SetObject(int column, CObject* pObject, BOOL serializeOnly = FALSE) const;
		RN1UTILS_API BOOL SetBinaryForCopy(int column, void *pValue, int length) const;

	RN1UTILS_API BOOL IsOpen();
	RN1UTILS_API BOOL Open(BOOL notDistinct = TRUE);
	RN1UTILS_API int GetRecordCount(BOOL notDistinct = TRUE);
	RN1UTILS_API BOOL OpenForLists(UDWORD numRows = 0, BOOL notDistinct = TRUE, HWND hMsgWnd = NULL);
	RN1UTILS_API BOOL OpenForUpdateOnly();
	RN1UTILS_API void Close();
	RN1UTILS_API void Reset();

	RN1UTILS_API BOOL MoveNext();	// must be open

	RN1UTILS_API BOOL Add(SDWORD* pNumRowsAdded = NULL);		// must be open for update
	RN1UTILS_API BOOL Delete(const CString& deleteWhereClause); // doesn't have to be open
	RN1UTILS_API BOOL Delete(const CString& deleteWhereClause, SDWORD* pNumRowsDeleted); // doesn't have to be open
	RN1UTILS_API BOOL Update(const CString& updateWhereClause);	// must be open for update
	RN1UTILS_API BOOL Update(const CString& updateWhereClause, SDWORD* pNumRowsUpdated);	// must be open for update

	RN1UTILS_API BOOL BeginTransaction();
	RN1UTILS_API BOOL CommitTransaction();
	RN1UTILS_API BOOL RollbackTransaction();

	RN1UTILS_API void SetSilentMode(BOOL mode) { silentMode = mode; }
	RN1UTILS_API BOOL GetSilentMode() const { return silentMode; }
	RN1UTILS_API void SetAfxNotPresent(BOOL mode) { afxNotPresent = mode; }
	RN1UTILS_API BOOL GetAfxNotPresent() { return afxNotPresent; }
	RN1UTILS_API CString GetLastError();

	DataSourceInfo* GetDataSourceInfo() const { return pDataSourceInfo; }
	CString GetSystemName() const { return pDataSourceInfo->connectionName; }
	CString GetDataSource() const { return pDataSourceInfo->dataSource; }
	CString GetUserName() const { return pDataSourceInfo->userName; }
	CString GetPassword() const { return pDataSourceInfo->password; }

	RN1UTILS_API BOOL AddColumnDef(const CString& newColumn, RFIELD rtype, int length = 0, BOOL notNull = FALSE);
	RN1UTILS_API BOOL AddColumnDef(const CString& oldName, const CString& newName, RFIELD newType, int length = 0, BOOL notNull = FALSE, long oldLength = -1);

	RN1UTILS_API BOOL CreateTable(BOOL bUseParser = TRUE);

	RN1UTILS_API BOOL DropTable();
	RN1UTILS_API BOOL RenameTable(const CString& newTableName);
	RN1UTILS_API BOOL RenameColumn(const CString& oldName, const CString& newName);
	RN1UTILS_API BOOL ModifyColumnVarToClobOracle(const CString& columnName, RFIELD rtype);
	RN1UTILS_API BOOL ModifyColumnIntToTextOracle(const CString& columnName, RFIELD rtype, int length);
	RN1UTILS_API BOOL ModifyColumn(const CString& columnName, RFIELD rtype, int length = 0, BOOL notNull = FALSE);
	RN1UTILS_API BOOL DropColumn(const CString& columnName);
	RN1UTILS_API BOOL AlterTable(const CString& newName, BOOL bIsFieldResizeOnly = FALSE);
	RN1UTILS_API BOOL AlterTableAddColumns();
	RN1UTILS_API BOOL CreatePrimaryKey();
	RN1UTILS_API BOOL DropPrimaryKey();
	RN1UTILS_API BOOL CreateIndex(const CString& indexName, BOOL unique);
	RN1UTILS_API BOOL DropIndex(const CString& indexName);

	RN1UTILS_API BOOL TableExists();
	RN1UTILS_API CString GetTableOwner();
	RN1UTILS_API CString GetFullTableName();
	RN1UTILS_API BOOL ColumnExists(const CString& columnName);
	RN1UTILS_API BOOL DoTablesExist();
	RN1UTILS_API BOOL GetTableList(CStringArray& tables);
	RN1UTILS_API BOOL GetColumnList(CRColumnArray& columns);
	RN1UTILS_API BOOL GetIndexList(CRIndexArray& indexes);
	RN1UTILS_API BOOL GetIndexList(CRIndexArray& indexes, BOOL bDoReplicate = FALSE);
	RN1UTILS_API CString GetPrimaryKeyCol();

	RN1UTILS_API BOOL GetTableStruct(CTableStruct& tableStruct);

	RN1UTILS_API BOOL DoSQL(const CString& sqlString, BOOL silentMode = FALSE, BOOL noErrors = FALSE);
	RN1UTILS_API BOOL OpenSQL(const CString& sqlString, BOOL silentMode = FALSE, BOOL noErrors = FALSE);
	RN1UTILS_API BOOL FetchRecord();
	RN1UTILS_API BOOL GetDataText(int col, RFIELD rtype, void* pBuf, long* pDataLength = NULL);
	RN1UTILS_API BOOL GetDataText(int col, CString& value);

	RN1UTILS_API BOOL SerializeOutForReplicate(CArchive& archive, BOOL doStructure, CRn1ProcessRequestStatus* pStatus = NULL);
	RN1UTILS_API BOOL SerializeInForReplicate(CArchive& archive, BOOL doStructure, CRn1ProcessRequestStatus* pStatus = NULL, void* pInfo = NULL, BOOL bIgnoreAdd = FALSE);  //*a
	RN1UTILS_API BOOL SerializeStructureIn(CArchive& archive);
	RN1UTILS_API BOOL SerializeRowIn(CArchive& archive);
	BOOL BCPSerializeRowIn(CArchive& archive, CBCPRecordSet* pBCPData, BOOL addRow);
	RN1UTILS_API static BOOL SerializeOutTag(CArchive& archive, int tag, int len, BYTE* pBuf);
	RN1UTILS_API static BOOL SerializeInTag(CArchive& archive, int& tag, int& len);
	RN1UTILS_API BOOL BCPReplicate(DataSourceInfo* pTargetDataSourceInfo);

	RN1UTILS_API BOOL SerializeStructureOutForConfig(CArchive& archive) const;
	RN1UTILS_API BOOL SerializeStructureInForConfig(CArchive& archive);
	RN1UTILS_API BOOL SerializeRowOut(CArchive& archive) const;

	RN1UTILS_API void SetDataSourceInfo(DataSourceInfo* pDataSourceInfo);

	RN1UTILS_API static CString SQLLogFileName();
	RN1UTILS_API void shiftLVar();
	RN1UTILS_API void restoreLVar();

	RN1UTILS_API BOOL IsOracle();
protected:
	CString tableName;
	CString whereClause;
	CString orderBy;
	CString groupBy;
	BOOL join;

	CRColumnArray cols, fetchCols;
	BOOL open;
	BOOL binaryColumns;
	int firstBinary;
	BOOL openForUpdate;
	CParamArray pParams;		// J*C

	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;

	HDBC hdbcUpdate;
	HSTMT hstmtUpdate;
	int updateState;

	DataSourceInfo* pDataSourceInfo;

	bool	binaryColumnAdded;

protected:
	BOOL AllocateColumn(CRColumn* pColumn);
	BOOL BindColumn(CRColumn* pColumn);
	int GetParamNum(int pos) const;
	BOOL BindParameter(CRColumn* pCol, int pos = -1) const;
public:
	BOOL BindOutputParameter(CRColumn* pCol, int paramNum);
	RN1UTILS_API BOOL CreateSP(const CString& spName, const CString& spText, BOOL replaceOld);
		BOOL BindParameterForCopy(CRColumn *pCol) const;
		BOOL CopyBinaries(CRRecordSet *pSource);

	static BOOL CheckODBCError(RETCODE rc, HDBC hdbc, HSTMT hstmt, TCHAR* psRoutine, BOOL noDialog = FALSE);
	static BOOL DoConnect(HDBC& hdbc, DataSourceInfo* pDataSourceInfo);

	BOOL BaseSetAppRole(HDBC hdbc, const CString& appRoleName, const CString& appRolePwd);
	RN1UTILS_API BOOL CollectTypeInfo(BOOL bIsDBUnicode);

protected:
	BOOL CheckODBCError(RETCODE rc, TCHAR* psRoutine) const;
	BOOL CheckODBCError(RETCODE rc, HDBC& hdbc, TCHAR* psRoutine) const;
	BOOL CollectTypeInfo();
	BOOL DoConnect(HDBC& hdbc);
	BOOL GetNewStatement(HDBC* phdbc, HSTMT* phstmt, BOOL exclusiveListConn = FALSE, HWND hMsgWnd = NULL);
	BOOL FreeStatement(HDBC& hdbc, HSTMT& hstmt);

	BOOL AddTransactionConnection(CRConnection*& pConnection);
	BOOL RemoveTransactionConnection();

	static CString GetDataSource(DataSourceInfo* pDataSourceInfo);
	static CString GetDatabase(DataSourceInfo* pDataSourceInfo);
	static CString GetUserName(DataSourceInfo* pDataSourceInfo);
	static CString GetPassword(DataSourceInfo* pDataSourceInfo);

	static BOOL ConnectOptions(DataSourceInfo* pDataSourceInfo, CRRecordSet* pRecordSet, HDBC hdbc);

	CString GetTypedColumnList();
	CString GetTypedColumn(int i);
	CString GetOldColumnList() const;
	CString GetNewColumnList() const;
	BOOL OracleCopyMemos(const CString& fromTableName, const CString& toTableName, const CString& fromPk, const CString& toPk, BOOL fromOldNames);
	BOOL DoesRelationshipUsersExist() const;
	BOOL DoesRelationshipRoleExist() const;

	void AddedColumnTypeCheck(RFIELD type);
public:
	BOOL AddColumnIfExists(CString csColumn, RFIELD rtype);
	RN1UTILS_API CRTypeInfo* GetTypeInfo(RFIELD rtype) const;
	RN1UTILS_API int GetConversionType(RFIELD fromType, RFIELD toType) const;
	RN1UTILS_API int GetTypeLength(RFIELD rtype) const;

	RN1UTILS_API static DWORD WaitForSyncObject(HANDLE hObject, HWND hMessageWnd);

#ifdef _DEBUG
	static void DumpLongString(CString longOne);
#else
#define DumpLongString(x)
#endif
	CString OutputParamValues(BOOL skipNulls);

	friend class CRCreate;
protected:
	U32 m_nBindingArraySize;	// Size of array for binding
	U32 m_nArrayIndex;			// Number of processing row (in array binding realization)
	U32 m_nNumRows;				// Rows number in the table

	CString m_strSchema;

public:
	int m_nLogSQL;

	RN1UTILS_API BOOL SetBinding(BOOL bBind, BOOL bBulk = FALSE);

	RN1UTILS_API static BOOL ConnectToDataSource(const CString& dataSource, const CString& database, const CString& userName, const CString& password, CRRecordSet* pRecordSet);
	RN1UTILS_API static BOOL Disconnect(DataSourceInfo* pDataSourceInfo);
	RN1UTILS_API CString  ParseSQLString(CString strSQLString);
	void DumpSQLString(LPCTSTR strSQLString);
	RN1UTILS_API BOOL OpenSQLForParser(const CString& sqlString, BOOL silentMode = FALSE, BOOL noErrors = FALSE);
	RN1UTILS_API BOOL OpenWithoutParsing(BOOL notDistinct = TRUE);
	static BOOL IsDictTableOrView(const CString& strName);
	RN1UTILS_API BOOL ChangeSQLDatabase(CString strDatabase);
	static BOOL CreateOraclePackage(CRRecordSet* pRecordSet);


	RN1UTILS_API BOOL FillBCPRowIn(const CBCPColumnArray& bcpCols);

	RN1UTILS_API BOOL ForceAdd();

	RN1UTILS_API void SetSchema(const CString& cstrSchema)
	{
		m_strSchema = cstrSchema;
		m_strSchema.MakeUpper();
	}

	RN1UTILS_API CString GetSchema()
	{
		return m_strSchema;
	}

private:
	BOOL IsTimeToExecute() const;


public:
	BOOL IsBindArray() const;
	


};
