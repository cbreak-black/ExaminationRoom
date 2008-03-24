-- Questionnaire library
Scene:log("Questionnaire Library loaded");

questions = {
	createQuestion = function (this, title, textLeft, textRight, numOptions)
		if type(title) == "string" and
			type(textLeft) == "string" and
			type(textRight) == "string" and
			type(numOptions) == "number" then
			return {
				["title"] = title;
				["textL"] = textLeft;
				["textR"] = textRight;
				["count"] = numOptions;
			};
		else
			return nil;
		end;
	end;

	loadQuestion = function (this, question)
		if question then
			local xStart = (question.count/2+1)*-1.5;
			-- Title
			local t = this:getText(1);
			t:setText(question.title);
			t:setPosition(1.5+xStart, 1.25, 0);
			Scene:addObject(t);
			-- Left Text
			t = this:getText(2);
			t:setText(question.textL);
			t:setPosition(1.5+xStart, 0.75, 0);
			Scene:addObject(t);
			-- Right Text
			t = this:getText(3);
			t:setText(question.textR);
			t:setPosition(1.5*question.count+xStart, 0.75, 0);
			Scene:addObject(t);
			-- Boxes
			for i = 1, question.count do
				local b = this:getBox(i);
				b:setPosition(xStart + i*1.5, -0.5, 0);
				Scene:addObject(b);
				-- Box Labels
				t = this:getText(3+i);
				t:setText(i);
				t:setPosition(xStart + i*1.5+0.25, -0.25, 0.25);
				t:setColor(1,1,1,0.5);
				Scene:addObject(t);
			end;
		end;
	end;

	unloadQuestion = function (this)
		for i, o in ipairs(this.boxes) do
			Scene:removeObject(o);
		end;
		for i, o in ipairs(this.texts) do
			Scene:removeObject(o);
		end;
	end;

	startQuestioning = function (this, questionList, callback)
		Scene:log("Questions Start");
		this.endCallback = callback;
		this.questionList = questionList;
		this.questionNumber = 1;
		this:registerEvents();
		local question = this.questionList[this.questionNumber];
		if question then
			this:loadQuestion(question);
		else
			this:endQuestioning();
		end;
	end;

	endQuestioning = function (this)
		Scene:log("Questions End");
		this:unloadQuestion()
		-- Clean temporaries
		this.quesitonList = nil;
		-- Call the callback
		this.endCallback();
	end;

	nextQuestion = function (this)
		local question = this.questionList[this.questionNumber];
		Scene:log("Finished Question: "..question.title);
		Scene:log("Question Reply: "..this.selected);
		this.selected = nil;
		this:unloadQuestion();
		this.questionNumber = this.questionNumber + 1;
		local question = this.questionList[this.questionNumber];
		if question then
			this:loadQuestion(question);
		else
			this:endQuestioning();
		end;
	end;

	-- IO
	parseInput = function (k)
		local this = questions; -- Static method
		local question = this.questionList[this.questionNumber];
		-- Number
		local num = string.byte(k);
		local one = 49;
		local max = one + question.count;
		if num >= one and num < max then
			local b;
			if this.selected then
				b = this:getBox(this.selected);
				b:setWireframe(true);
			end;
			this.selected = num-one+1;
			b = this:getBox(this.selected);
			b:setWireframe(false);
		elseif num == 4 or num == 5 then
			if this.selected then
				this:nextQuestion();
			end;
		else
			Scene:log("Input invalid, ignored ("..string.byte(k)..")");
		end
	end;

	registerEvents = function (this)
		Scene:setEventListener("keyDown", this.parseInput);
	end;

	unregisterEvents = function (this)
		Scene:setEventListener("keyDown", nil);
	end;

	-- Object Management
	getBox = function (this, idx)
		local b;
		if this.boxes[idx] then
			b = this.boxes[idx];
		else
			b = Object("Rectangle");
			this.boxes[idx] = b;
		end
		b:setWireframe(true);
		b:setColor(1,1,1,1);
		b:setDirA(1, 0, 0);
		b:setDirB(0, 1, 0);
		return b;
	end;

	getText = function (this, idx)
		if this.texts[idx] == nil then
			local t = Object("Text");
			this.texts[idx] = t;
		end
		return this.texts[idx];
	end;

	-- Objects
	boxes = {};
	texts = {};
}