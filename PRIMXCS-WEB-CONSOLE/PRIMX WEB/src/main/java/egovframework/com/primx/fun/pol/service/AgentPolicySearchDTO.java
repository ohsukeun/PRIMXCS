package egovframework.com.primx.fun.pol.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import egovframework.com.primx.fun.cmm.util.DateUtil;
import lombok.Data;

@Data
public class AgentPolicySearchDTO extends DataTableBaseVO {
    private String searchDateType;
    private String searchStartDate;
    private String searchEndDate;
    private String searchPolicyType;
    private String searchCondition;
    private String searchKeyword;

    public void convertUserTimeToUTC() {
        this.searchStartDate = DateUtil.convertUserTimeToUTC(this.searchStartDate);
        this.searchEndDate = DateUtil.convertUserTimeToUTC(this.searchEndDate);
    }
    
} 