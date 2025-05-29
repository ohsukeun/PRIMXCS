package egovframework.com.primx.fun.log.web;

import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.primx.fun.log.service.PrimxLogService;
import egovframework.com.primx.fun.usr.service.PrimxUserService;
import egovframework.com.primx.fun.log.service.AgentProductsSearchVO;
import egovframework.com.primx.fun.log.service.ManageLogSearchVO;

/**
 * 로그 관리 관련 기능
 * @author 김영우
 * @since 2024.12.3
 * @version 1.0
 * @see
 *
 * <pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일      수정자          수정내용
 *  -------    --------    ---------------------------
 *  2024.12.03  김영우          최초 생성
 *
 *  </pre>
 */
//'ADMIN','SUPER'
@Controller
@RequestMapping(value = "/primx/fun/log")
public class PrimxLogController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxLogService")
	private PrimxLogService primxLogService;

	@Resource(name = "primxUserService")
	private PrimxUserService primxUserService;
	

	@RequestMapping(value = "/manageLog/view.do")
	public String getManageLogView(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/log/operationHistory";
	}
	
	@ResponseBody
	@RequestMapping(value = "/manageLog/list.do")
	public Map<String, Object> getManageLogList(@ModelAttribute ManageLogSearchVO searchVO,
			@RequestParam Map<String, String> params) throws Exception {
		
		// 검색 파라미터 설정
		searchVO.setSearchDateType(params.get("searchDateType"));
		searchVO.setSearchStartDate(params.get("searchStartDate"));
		searchVO.setSearchEndDate(params.get("searchEndDate"));
		searchVO.setSearchCategory(params.get("searchCategory"));
		searchVO.setSearchType(params.get("searchType"));
		searchVO.setSearchCondition(params.get("searchCondition"));
		searchVO.setSearchKeyword(params.get("searchKeyword"));

		searchVO.convertUserTimeToUTC();

		return primxLogService.selectListManageLog(searchVO);
	}

	//'USER','ADMIN','SUPER'
	@RequestMapping(value = "/productLog/view.do")
	public String getProductLogView(ModelMap model) throws Exception {

		return "egovframework/com/primx/fun/log/productHistory";
	}

	//'USER','ADMIN','SUPER'
	@ResponseBody
	@RequestMapping(value = "/productLog/list.do")
	public Map<String, Object> getProductLogList(@ModelAttribute AgentProductsSearchVO searchVO,
			@RequestParam Map<String, String> params) throws Exception {
		
		// 검색 파라미터 설정
		searchVO.setSearchDateType(params.get("searchDateType"));
		searchVO.setSearchStartDate(params.get("searchStartDate"));
		searchVO.setSearchEndDate(params.get("searchEndDate"));
		searchVO.setSearchLevel(params.get("searchLevel"));
		searchVO.setSearchProduct(params.get("searchProduct"));
		searchVO.setSearchType(params.get("searchType"));
		searchVO.setSearchCondition(params.get("searchCondition"));
		searchVO.setSearchKeyword(params.get("searchKeyword"));

		searchVO.convertUserTimeToUTC();
		
		return primxLogService.selectListProductLog(searchVO);
	}

}
