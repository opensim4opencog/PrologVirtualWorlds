package jamud.object;

import jamud.JamudException;

public class TemplateConflictException extends JamudException {


    private MudObject original;

    public MudObject getOriginal() {
	return original;
    }


    private MudObject problem;

    public MudObject getProblem() {
	return problem;
    }


    private String tid;

    public String getTemplateId() {
	return tid;
    }



    public TemplateConflictException(MudObject original, MudObject problem,
				     String tid) {
	super( "Template Conflict on TID '".concat(tid).concat("'") );
	this.original = original;
	this.problem = problem;
	this.tid = tid;
    }

}
