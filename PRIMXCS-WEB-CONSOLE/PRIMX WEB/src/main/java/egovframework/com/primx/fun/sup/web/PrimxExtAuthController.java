package egovframework.com.primx.fun.sup.web;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.cmm.EgovMessageSource;
import egovframework.com.primx.fun.sup.service.PrimxExtAuthService;
import egovframework.com.primx.fun.sup.service.SmartContractDTO;

@Controller
@RequestMapping(value = "/primx/fun/sup")
public class PrimxExtAuthController {

	@Resource(name = "egovMessageSource")
	private EgovMessageSource egovMessageSource;

	@Resource(name = "primxExtAuthService")
	private PrimxExtAuthService primxExtAuthService;

	@ResponseBody
	@RequestMapping(value = "/applyXZEDSmartContractSetting.do", method = RequestMethod.POST)
	public Map<String, Object> applyXZEDSmartContractSetting(@RequestBody List<SmartContractDTO> smartContractDTOList) {
		
		Map<String, Object> result = new HashMap<>();
		try {
			result = primxExtAuthService.applyXZEDSmartContractSetting(smartContractDTOList);
		} catch (Exception e) {
			result.put("result", "fail");
			result.put("message", e.getMessage());
		}
		return result;
	}

	// 조회기능 파라미터로 emplyrId, agentId, fileName 받아서 조회
	@ResponseBody
	@RequestMapping(value = "/checkSmartContractSetting.do", method = RequestMethod.POST)
	public Map<String, Object> checkSmartContractSetting(@RequestBody SmartContractDTO smartContractDTO) {
		return primxExtAuthService.checkSmartContractSetting(smartContractDTO);
	}
}
