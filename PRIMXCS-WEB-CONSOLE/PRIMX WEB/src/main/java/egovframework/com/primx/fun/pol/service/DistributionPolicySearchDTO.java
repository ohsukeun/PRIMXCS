package egovframework.com.primx.fun.pol.service;

import egovframework.com.primx.fun.cmm.service.DataTableBaseVO;
import lombok.Data;

@Data
public class DistributionPolicySearchDTO extends DataTableBaseVO {
    private String searchDeploymentStatus;
    private String searchDateType;
    private String searchStartDate;
    private String searchEndDate;
    private String searchDeptId;
    private String searchDeptName;
    private String searchProductName;
    private int searchProcessorArchitecture;
}