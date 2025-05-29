package egovframework.com.primx.fun.cmm.service.impl;

import java.util.List;

import org.springframework.stereotype.Repository;

import egovframework.com.cmm.service.impl.EgovComAbstractDAO;
import egovframework.com.primx.fun.cmm.service.DeptSearchVO;
import egovframework.com.primx.fun.cmm.service.DeptVO;
import egovframework.com.primx.fun.cmm.service.UserInfoVO;
import egovframework.com.primx.fun.cmm.service.MyPageUpdateDTO;

@Repository("commonDAO")
public class CommonDAO extends EgovComAbstractDAO {

	public List<DeptVO> selectDeptTree(DeptSearchVO searchVO) {
		return selectList("commonDAO.selectDeptTree", searchVO);
	}
	
	public UserInfoVO selectUserInfo(String userId) {
        return (UserInfoVO) selectOne("commonDAO.selectUserInfo", userId);
	}
	//회원 정보 수정
	public void updateUserInfo(MyPageUpdateDTO updateDTO) {
		update("commonDAO.updateUserInfo", updateDTO);
	}
}