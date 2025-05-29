package egovframework.com.primx.fun.log.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class ManageLogSearchVO extends DataTableBaseVO {
    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private String searchDateType;
    private String searchStartDate;
    private String searchEndDate;
    private String searchCategory;    // 분류
    private String searchType;        // 종류
    private String searchCondition;   // 검색조건
    private String searchKeyword;     // 검색어

    public void convertUTCtoUserTime() {
        this.searchStartDate = DateUtil.convertUTCtoUserTime(this.searchStartDate);
        this.searchEndDate = DateUtil.convertUTCtoUserTime(this.searchEndDate);
    }

    public void convertUserTimeToUTC() {
        this.searchStartDate = DateUtil.convertUserTimeToUTC(this.searchStartDate);
        this.searchEndDate = DateUtil.convertUserTimeToUTC(this.searchEndDate);
    }
} 