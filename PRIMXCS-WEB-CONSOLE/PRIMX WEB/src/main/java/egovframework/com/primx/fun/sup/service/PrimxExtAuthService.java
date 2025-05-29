package egovframework.com.primx.fun.sup.service;

import java.util.List;
import java.util.Map;

public interface PrimxExtAuthService {
    Map<String, Object> applyXZEDSmartContractSetting(List<SmartContractDTO> smartContractDTOList) throws Exception;
    Map<String, Object> checkSmartContractSetting(SmartContractDTO smartContractDTO);
}
