package egovframework.com.primx.fun.sup.service.impl;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.sup.service.SmartContractDTO;

@Repository("extAuthDAO")
public class ExtAuthDAO extends EgovComAbstractDAO {

    // 스마트 컨트랙트 설정 등록
    public int applyXZEDSmartContractSetting(SmartContractDTO smartContractDTO) {
        return insert("extAuthDAO.applyXZEDSmartContractSetting", smartContractDTO);
    }

    // 스마트 컨트랙트 설정 조회
    public SmartContractDTO checkSmartContractSetting(SmartContractDTO smartContractDTO) {
        return selectOne("extAuthDAO.checkSmartContractSetting", smartContractDTO);
    }
} 