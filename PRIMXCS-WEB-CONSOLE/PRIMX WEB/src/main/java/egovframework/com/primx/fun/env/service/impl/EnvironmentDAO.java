package egovframework.com.primx.fun.env.service.impl;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.env.service.EnvGeneralVO;
import egovframework.com.primx.fun.env.service.EnvSystemSettingsVO;
import egovframework.com.primx.fun.lic.service.LicenseDetailVO;
import egovframework.com.primx.fun.lic.service.LicenseGeneralVO;

@Repository("environmentDAO")
public class EnvironmentDAO extends EgovComAbstractDAO {

	public EnvGeneralVO selectEnvGeneralById(Long id) {

		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return selectOne("environmentDAO.selectEnvGeneralById", paramMap);
	}

	public String selectServerConfigCompanyId() {
		return selectOne("environmentDAO.selectServerConfigCompanyId");
	}


//	@Query(value = "update T_SERVER_CONFIG set C_VALUE_DATA = :companyId where C_KEY_NAME='CompanyID'", nativeQuery = true)
	public int updateServerConfigCompanyId(String companyId) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("companyId", companyId);

		return update("environmentDAO.updateServerConfigCompanyId", paramMap);
	}

	public EnvSystemSettingsVO selectMacAddress(String macAddress) {

		Map<String, String> paramMap = new HashMap<>();
		paramMap.put("macAddress", macAddress);

		return selectOne("environmentDAO.selectMacAddress", paramMap);
	}

	public int insertEnvSystemSettings(EnvSystemSettingsVO vo) {
		return update("environmentDAO.insertEnvSystemSettings", vo);
	}

	public int updateEnvGeneral(EnvGeneralVO vo) {
		return update("environmentDAO.updateEnvGeneral", vo);
	}

	public int insertLicenseGeneral(LicenseGeneralVO vo) {
		return update("environmentDAO.insertLicenseGeneral", vo);
	}

	public int updateLicenseGeneral(LicenseGeneralVO vo) {
		return update("environmentDAO.updateLicenseGeneral", vo);
	}

	public LicenseGeneralVO selectLicenseGeneralById(Long id) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return selectOne("environmentDAO.selectLicenseGeneralById", paramMap);
	}

	public List<LicenseDetailVO> findByProductName(String productName) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("productName", productName);

		return selectList("environmentDAO.findByProductName", paramMap);
	}

	public int deleteById(Long id) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("id", id);

		return (Integer)delete("environmentDAO.deleteById", paramMap);
	}

	public int insertLicenseDetail(LicenseDetailVO vo) {
		return update("environmentDAO.insertLicenseDetail", vo);
	}

	public int selectLicenseGeneralByMacAddress(String macAddress) {
		Map<String, Object> paramMap = new HashMap<>();
		paramMap.put("macAddress", macAddress);

		Integer result = selectOne("environmentDAO.selectLicenseGeneralByMacAddress", paramMap);
		return (result != null) ? result : 0; // null인 경우 0 반환
	}
}
