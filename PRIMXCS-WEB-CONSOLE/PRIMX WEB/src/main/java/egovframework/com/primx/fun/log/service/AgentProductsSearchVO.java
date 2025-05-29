package egovframework.com.primx.fun.log.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class AgentProductsSearchVO extends DataTableBaseVO {
    private String searchDateType;
    private String searchStartDate;
    private String searchEndDate;
    private String searchLevel;
    private String searchProduct;
    private String searchType;
    private String searchCondition;
    private String searchKeyword;

    public void convertUTCtoUserTime() {
        this.searchStartDate = DateUtil.convertUTCtoUserTime(this.searchStartDate);
        this.searchEndDate = DateUtil.convertUTCtoUserTime(this.searchEndDate);
    }

    public void convertUserTimeToUTC() {
        this.searchStartDate = DateUtil.convertUserTimeToUTC(this.searchStartDate);
        this.searchEndDate = DateUtil.convertUserTimeToUTC(this.searchEndDate);
    }
} 