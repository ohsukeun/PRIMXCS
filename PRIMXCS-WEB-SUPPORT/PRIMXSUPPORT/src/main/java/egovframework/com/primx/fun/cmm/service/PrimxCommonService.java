package egovframework.com.primx.fun.cmm.service;

import java.util.Map;

public interface PrimxCommonService {
    
	   public UserInfoVO getMyPageInfo(String userId) throws Exception;

	   public Map<String, Object> updateMyPageInfo(MyPageUpdateDTO updateDTO) throws Exception;
	}


