#include "KausTaggedWidget.h"

bool UKausTaggedWidget::HasTag(FGameplayTag TagToCheck) const
{
	return WidgetTags.HasTag(TagToCheck);
}
