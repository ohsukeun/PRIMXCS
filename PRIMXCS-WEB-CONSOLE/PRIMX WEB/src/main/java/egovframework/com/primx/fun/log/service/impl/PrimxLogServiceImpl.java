package egovframework.com.primx.fun.log.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Service;
import egovframework.com.primx.fun.log.service.AgentProductLogDTO;
import egovframework.com.primx.fun.log.service.ManageLogDTO;
import egovframework.com.primx.fun.log.service.ManageLogEntityDTO;
import egovframework.com.primx.fun.log.service.PrimxLogService;
import egovframework.com.primx.fun.log.service.AgentProductsSearchVO;
import egovframework.com.primx.fun.log.service.ManageLogSearchVO;

@Service("primxLogService")
public class PrimxLogServiceImpl implements PrimxLogService {

	@Resource(name = "logDAO")
	private LogDAO logDAO;

	@Override
	public Map<String, Object> selectListManageLog(ManageLogSearchVO searchVO) throws Exception {
		List<ManageLogEntityDTO> entityResult = logDAO.selectListManageLog(searchVO);
		int totalCnt = logDAO.selectListManageLogCnt(searchVO);

		List<ManageLogDTO> result = new ArrayList<>();
		// 여기서 함수 만들어서 작업 시작
		
		System.out.println("################## entityResult:" + entityResult.size());
	    for (ManageLogEntityDTO entity : entityResult) {
			entity.convertUTCtoUserTime();
	        // 메시지를 가공한 DTO 생성
	        ManageLogDTO dto = processManageLogMessage(entity);
	        result.add(dto);
	    }
		System.out.println("################## END");
		
		Map<String, Object> response = new HashMap<>();
		response.put("draw", searchVO.getDraw());
		response.put("recordsTotal", totalCnt);
		response.put("recordsFiltered", totalCnt);
		response.put("data", result);
		
		return response;
	}


	/**
	 * 메시지를 가공하여 DTO를 생성하는 메서드
	 */
	private ManageLogDTO processManageLogMessage(ManageLogEntityDTO entity) {
	    // 1. 템플릿 메시지 가져오기
	    String messageTemplate = entity.getMessage();
	    String columnGuide = entity.getColumnGuide();
	
	    // 2. columnGuide를 분리하여 컬럼명을 가져옴
	    if (columnGuide == null || columnGuide.isEmpty()) {
	        return createManageLogDTO(entity, messageTemplate); // columnGuide가 없으면 그대로 반환
	    }
	    
	    String[] columns = columnGuide.split("\\|");
	    String processedMessage = messageTemplate;
	    
	    System.out.println("################## columnGuide:" + columnGuide +", messageTemplate:"+messageTemplate
	    		+", columns.length:"+columns.length);
	
	    // 3. 각 컬럼 값을 추출하여 메시지 치환
	    for (int i = 0; i < columns.length; i++) {
	        String placeholder = "__VALUE" + (i + 1) + "__";
	        String columnValue = getColumnValue(entity, columns[i]); // 컬럼 값 가져오기
	        System.out.println("####################columnValue:"+columnValue+", columns["+i+"]:"+columns[i]);
	        processedMessage = processedMessage.replace(placeholder, columnValue != null ? columnValue : "");
	    }
	
	    // 4. DTO 생성 및 반환
	    return createManageLogDTO(entity, processedMessage);
	}
	
	/**
	 * 컬럼 값을 추출하는 메서드
	 */
	private String getColumnValue(ManageLogEntityDTO entity, String columnName) {
	    switch (columnName) {
	        case "C_USER_ID": return entity.getUserId();
	        case "C_USER_NAME": return entity.getUserName();
	        case "C_USER_IP": return entity.getUserIp();
	        case "C_AGENT_ID": return entity.getAgentId();
	        case "C_AGENT_IP": return entity.getAgentIp();
	        case "C_COMPUTER_NAME": return entity.getComputerName();
	        case "C_PRODUCT_POLICY_ID": return entity.getProductPolicyId();
	        case "C_PRODUCT_POLICY_NAME": return entity.getProductPolicyName();
	        case "C_AGENT_POLICY_ID": return entity.getAgentPolicyId();
	        case "C_AGENT_POLICY_NAME": return entity.getAgentPolicyName();
	        case "C_RESERVED1": return entity.getReserved1();
	        case "C_RESERVED2": return entity.getReserved2();
	        case "C_RESERVED3": return entity.getReserved3();
	        case "C_RESERVED4": return entity.getReserved4();
	        case "C_RESERVED5": return entity.getReserved5();
	        case "C_RESERVED6": return entity.getReserved6();
	        default: return null; // 매칭되지 않는 컬럼은 null 반환
	    }
	}

	
	/**
	 * ManageLogDTO 생성 메서드
	 */
	private ManageLogDTO createManageLogDTO(ManageLogEntityDTO entity, String processedMessage) {
	    ManageLogDTO dto = ManageLogDTO.builder()
			.seqNo(entity.getSeqNo())
			.logTypeName(entity.getLogTypeName())
			.logName(entity.getLogName())
			.message(processedMessage)
			.columnGuide(entity.getColumnGuide())
			.computerName(entity.getComputerName())
			.userName(entity.getUserName())
			.deptName(entity.getDeptName())
			.manageName(entity.getManageName())
			.logDate(entity.getLogDate())
			.logTime(entity.getLogTime())
			.build();
	    return dto;
	}


	@Override
	public Map<String, Object> selectListProductLog(AgentProductsSearchVO searchVO) throws Exception {
		List<AgentProductLogDTO> result = logDAO.selectListAgentProductsLog(searchVO);
		int totalCnt = logDAO.selectListAgentProductsLogCnt(searchVO);

		for(AgentProductLogDTO dto : result) {
			dto.convertUTCtoUserTime();
		}

		Map<String, Object> response = new HashMap<>();
		response.put("draw", searchVO.getDraw());
		response.put("recordsTotal", totalCnt);
		response.put("recordsFiltered", totalCnt);
		response.put("data", result);
		
		return response;
	}

}
